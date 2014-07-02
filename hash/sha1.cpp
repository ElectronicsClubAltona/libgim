/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2013 Danny Robson <danny@nerdcruft.net>
 */

#include "sha1.hpp"

#include "bitwise.hpp"
#include "debug.hpp"
#include "endian.hpp"
#include "types.hpp"
#include "types/casts.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>


using util::hash::SHA1;
using std::numeric_limits;
using std::begin;
using std::end;


// Logical function for sequence of rounds
static inline uint32_t
f_00 (uint32_t B, uint32_t C, uint32_t D)
    { return (B & C) | (~B & D); }


static inline uint32_t
f_20 (uint32_t B, uint32_t C, uint32_t D)
    { return B ^ C ^ D; }


static inline uint32_t
f_40 (uint32_t B, uint32_t C, uint32_t D)
    { return (B & C) | (B & D) | (C & D); }


static inline uint32_t
f_60 (uint32_t B, uint32_t C, uint32_t D)
    { return B ^ C ^ D; }


//-----------------------------------------------------------------------------
// Constant words for sequence of rounds
static const uint32_t K_00 = 0x5A827999;
static const uint32_t K_20 = 0x6ED9EBA1;
static const uint32_t K_40 = 0x8F1BBCDC;
static const uint32_t K_60 = 0xCA62C1D6;


static const uint32_t DEFAULT_H[] = {
    0x67452301,
    0xEFCDAB89,
    0x98BADCFE,
    0x10325476,
    0xC3D2E1F0
};

static const size_t BLOCK_WORDS = 16;
static const size_t BLOCK_BYTES = BLOCK_WORDS * sizeof (uint32_t);


//-----------------------------------------------------------------------------
SHA1::SHA1()
{
    reset ();
}


void
SHA1::reset (void) {
    total = 0;
    state = READY;

    std::copy (std::begin (DEFAULT_H), std::end (DEFAULT_H), H);
}


//-----------------------------------------------------------------------------
void
SHA1::update (const uint8_t *data, size_t size) {
    CHECK_EQ (state, READY);
    CHECK_GE (numeric_limits<decltype(total)>::max () - total, size);

    while (size > 0) {
        // Copy the data into the remaining available buffer slots
        const size_t offset = total % BLOCK_BYTES;
        const size_t chunk  = std::min (BLOCK_BYTES - offset, size);

        std::copy (data, data + chunk, c + offset);

        total += chunk;

        // Attempt to process if full
        if (total % BLOCK_BYTES == 0)
            process ();

        size -= chunk;
        data += chunk;
    }
}


//-----------------------------------------------------------------------------
void
SHA1::process (void) {
    CHECK_EQ (total % BLOCK_BYTES, 0);

    // Byteswap the raw input we have buffered ready for arithmetic
    std::transform (std::begin (W),
                    std::end   (W),
                    std::begin (W),
                    [] (uint32_t x) {
        return ntoh (x);
    });

    // Initialise the work buffer and the state variables
    for (size_t t = 16; t < 80; ++t)
        W[t] = rotatel (W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16], 1);

    uint32_t A = H[0],
             B = H[1],
             C = H[2],
             D = H[3],
             E = H[4];

    // Perform each of the four rounds
    #define ROTATE_STATE(i) do {                                             \
        uint32_t temp = rotatel (A, 5) + f_##i (B, C, D) + E + W[t] + K_##i; \
        E = D;                                                               \
        D = C;                                                               \
        C = rotatel (B, 30);                                                 \
        B = A;                                                               \
        A = temp;                                                            \
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


//-----------------------------------------------------------------------------
void
SHA1::finish (void) {
    size_t offset = total % BLOCK_BYTES;
    size_t used   = total * 8;

    // Append a single one bit
    c[offset++] = 0x80;
    total += 1;

    // Zero fill if we can't append length
    size_t chunk = BLOCK_BYTES - offset;
    if (chunk < sizeof (total)) {
        std::fill_n (c + offset, chunk, 0);
        total += chunk;

        process ();

        chunk  = BLOCK_BYTES;
        offset = 0;
    }

    // Zero fill and append total length
    std::fill_n (c + offset, chunk - sizeof (total), 0);
    c[BLOCK_BYTES - 1] = used & 0xFF; used >>= 8;
    c[BLOCK_BYTES - 2] = used & 0xFF; used >>= 8;
    c[BLOCK_BYTES - 3] = used & 0xFF; used >>= 8;
    c[BLOCK_BYTES - 4] = used & 0xFF; used >>= 8;
    c[BLOCK_BYTES - 5] = used & 0xFF; used >>= 8;
    c[BLOCK_BYTES - 6] = used & 0xFF; used >>= 8;
    c[BLOCK_BYTES - 7] = used & 0xFF; used >>= 8;
    c[BLOCK_BYTES - 8] = used & 0xFF;

    total += chunk;
    process ();

    state = FINISHED;
}


//-----------------------------------------------------------------------------
SHA1::digest_t
SHA1::digest (void) const {
    CHECK_EQ (state, FINISHED);

    return { {
        size_cast<uint8_t> ((H[0] >> 24u) & 0xFF),
        size_cast<uint8_t> ((H[0] >> 16u) & 0xFF),
        size_cast<uint8_t> ((H[0] >>  8u) & 0xFF),
        size_cast<uint8_t> ((H[0]       ) & 0xFF),
        size_cast<uint8_t> ((H[1] >> 24u) & 0xFF),
        size_cast<uint8_t> ((H[1] >> 16u) & 0xFF),
        size_cast<uint8_t> ((H[1] >>  8u) & 0xFF),
        size_cast<uint8_t> ((H[1]       ) & 0xFF),
        size_cast<uint8_t> ((H[2] >> 24u) & 0xFF),
        size_cast<uint8_t> ((H[2] >> 16u) & 0xFF),
        size_cast<uint8_t> ((H[2] >>  8u) & 0xFF),
        size_cast<uint8_t> ((H[2]       ) & 0xFF),
        size_cast<uint8_t> ((H[3] >> 24u) & 0xFF),
        size_cast<uint8_t> ((H[3] >> 16u) & 0xFF),
        size_cast<uint8_t> ((H[3] >>  8u) & 0xFF),
        size_cast<uint8_t> ((H[3]       ) & 0xFF),
        size_cast<uint8_t> ((H[4] >> 24u) & 0xFF),
        size_cast<uint8_t> ((H[4] >> 16u) & 0xFF),
        size_cast<uint8_t> ((H[4] >>  8u) & 0xFF),
        size_cast<uint8_t> ((H[4]       ) & 0xFF)
    } };
}
