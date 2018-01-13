/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Copyright 2013 Danny Robson <danny@nerdcruft.net>
 */

#include "sha1.hpp"

#include "../iterator.hpp"
#include "../bitwise.hpp"
#include "../debug.hpp"
#include "../endian.hpp"
#include "../cast.hpp"

#include <algorithm>
#include <cstdint>
#include <limits>


using util::hash::SHA1;


///////////////////////////////////////////////////////////////////////////////
// Constant words for sequence of rounds
static constexpr uint32_t K_00 = 0x5A827999;
static constexpr uint32_t K_20 = 0x6ED9EBA1;
static constexpr uint32_t K_40 = 0x8F1BBCDC;
static constexpr uint32_t K_60 = 0xCA62C1D6;


//-----------------------------------------------------------------------------
static constexpr
std::array<uint32_t,5> INITIAL_H = {
    0x67452301,
    0xEFCDAB89,
    0x98BADCFE,
    0x10325476,
    0xC3D2E1F0
};


//-----------------------------------------------------------------------------
static constexpr size_t BLOCK_WORDS = 16;
static constexpr size_t BLOCK_BYTES = BLOCK_WORDS * sizeof (uint32_t);


///////////////////////////////////////////////////////////////////////////////
// Logical function for sequence of rounds
static constexpr
uint32_t
f_00 (uint32_t B, uint32_t C, uint32_t D)
{
    return (B & C) | (~B & D);
}


//-----------------------------------------------------------------------------
static constexpr
uint32_t
f_20 (uint32_t B, uint32_t C, uint32_t D)
{
    return B ^ C ^ D;
}


//-----------------------------------------------------------------------------
static constexpr
uint32_t
f_40 (uint32_t B, uint32_t C, uint32_t D)
{
    return (B & C) | (B & D) | (C & D);
}


//-----------------------------------------------------------------------------
static constexpr
uint32_t
f_60 (uint32_t B, uint32_t C, uint32_t D)
{
    return B ^ C ^ D;
}


///////////////////////////////////////////////////////////////////////////////
static void
process (std::array<uint32_t,5> &H, std::array<uint32_t,16+64> &W)
{
    // Byteswap the raw input we have buffered ready for arithmetic
    std::transform (std::begin (W),
                    std::end   (W),
                    std::begin (W),
                    [] (uint32_t x) {
                        return util::ntoh (x);
                    });

    // Initialise the work buffer and the state variables
    for (size_t t = 16; t < 80; ++t)
        W[t] = util::rotatel (W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16], 1);

    uint32_t A = H[0],
        B = H[1],
        C = H[2],
        D = H[3],
        E = H[4];

    // Perform each of the four rounds
#define ROTATE_STATE(i) do {                                                        \
        uint32_t temp = util::rotatel (A, 5) + f_##i (B, C, D) + E + W[t] + K_##i;  \
        E = D;                                                                      \
        D = C;                                                                      \
        C = util::rotatel (B, 30);                                                  \
        B = A;                                                                      \
        A = temp;                                                                   \
    } while (0)

    for (size_t t =  0; t < 20; ++t) ROTATE_STATE(00);
    for (size_t t = 20; t < 40; ++t) ROTATE_STATE(20);
    for (size_t t = 40; t < 60; ++t) ROTATE_STATE(40);
    for (size_t t = 60; t < 80; ++t) ROTATE_STATE(60);

    // Update the resulting hash state
    H[0] += A;
    H[1] += B;
    H[2] += C;
    H[3] += D;
    H[4] += E;
}


///////////////////////////////////////////////////////////////////////////////
SHA1::digest_t
SHA1::operator() (util::view<const uint8_t*> data) noexcept
{
    return (*this) (data, nullptr);
}


//-----------------------------------------------------------------------------
SHA1::digest_t
SHA1::operator() (
    util::view<const uint8_t*> data_a,
    util::view<const uint8_t*> data_b
) noexcept {
    /* RESET */
    uint64_t total = 0;

    union {
        std::array<uint32_t,5> w;
        std::array<uint8_t,20> b;
    } H;

    union {
        std::array<uint8_t,  16*4+64*4> c;
        std::array<uint32_t, 16  +64  > W;
    } state;

    H.w = INITIAL_H;

    /* UPDATE */
    transform_by_block (
        util::view {state.c.data (), BLOCK_BYTES },
        [&] (auto) { process (H.w, state.W); },
        data_a, data_b
    );

    total += data_a.size () + data_b.size ();

    /* FINISH */
    {
        size_t offset = total % BLOCK_BYTES;
        size_t used   = total * 8;

        // Append a single one bit
        state.c[offset++] = 0x80;
        total += 1;

        // Zero fill if we can't append length
        size_t chunk = BLOCK_BYTES - offset;
        if (chunk < sizeof (total)) {
            std::fill_n (std::begin (state.c) + offset, chunk, 0);
            total += chunk;

            process (H.w, state.W);

            chunk  = BLOCK_BYTES;
            offset = 0;
        }

        // Zero fill and append total length
        std::fill_n (std::begin (state.c) + offset, chunk - sizeof (total), 0);
        state.c[BLOCK_BYTES - 1] = used & 0xFF; used >>= 8;
        state.c[BLOCK_BYTES - 2] = used & 0xFF; used >>= 8;
        state.c[BLOCK_BYTES - 3] = used & 0xFF; used >>= 8;
        state.c[BLOCK_BYTES - 4] = used & 0xFF; used >>= 8;
        state.c[BLOCK_BYTES - 5] = used & 0xFF; used >>= 8;
        state.c[BLOCK_BYTES - 6] = used & 0xFF; used >>= 8;
        state.c[BLOCK_BYTES - 7] = used & 0xFF; used >>= 8;
        state.c[BLOCK_BYTES - 8] = used & 0xFF;

        total += chunk;
        process (H.w, state.W);
    }

    /* DIGEST */
    std::transform (std::begin (H.w), std::end (H.w), std::begin (H.w), util::hton<uint32_t>);
    return H.b;
}
