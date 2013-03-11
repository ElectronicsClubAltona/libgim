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


// Constant words for sequence of rounds
static const uint32_t K_00 = 0x5A827999;
static const uint32_t K_20 = 0x6ED9EBA1;
static const uint32_t K_40 = 0x8F1BBCDC;
static const uint32_t K_60 = 0xCA62C1D6;


static const uint32_t DEFAULT_H0 = 0x67452301;
static const uint32_t DEFAULT_H1 = 0xEFCDAB89;
static const uint32_t DEFAULT_H2 = 0x98BADCFE;
static const uint32_t DEFAULT_H3 = 0x10325476;
static const uint32_t DEFAULT_H4 = 0xC3D2E1F0;


static const size_t BLOCK_SIZE = 16;


SHA1::SHA1()
{
    reset ();
}


void
SHA1::reset (void) {
    total = 0;

    H[0] = DEFAULT_H0;
    H[1] = DEFAULT_H1;
    H[2] = DEFAULT_H2;
    H[3] = DEFAULT_H3;
    H[4] = DEFAULT_H4;

    std::fill (begin (W), end (W), 0);

    state = READY;
}


void
SHA1::update (const uint8_t *data, size_t size) {
    assert (state == READY);
    assert (numeric_limits<decltype(total)>::max () - total >= size);


    while (size > 0) {
        size_t offset = total % BLOCK_SIZE;
        size_t tocopy = std::min (BLOCK_SIZE - offset, size);
        
        for (size_t i = 0; i < tocopy; ++i) {
            size_t   octet = sizeof(W[0]) - (offset + i) % sizeof (W[0]);
            size_t   index = (offset / sizeof (W[0]) + i) / sizeof (W[0]);

            size_t   shift = (octet - 1) * 8u;
            uint32_t byte  = *data++;

            W[index] |= byte << shift;
        }

        total += tocopy;

        if (total % BLOCK_SIZE == 0) {
            process ();
            std::fill (begin (W), end (W), 0);
        }
        size -= tocopy;
    }
}


void
SHA1::process (void) {
    // Shuffle the work buffer a bit and initialise the state variables
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

    // Save out the intermediate hash again
    H[0] += A;
    H[1] += B;
    H[2] += C;
    H[3] += D;
    H[4] += E;
}


void
SHA1::finish (void) {
    size_t index = (total / sizeof (W[0])) % BLOCK_SIZE;
    size_t octet = sizeof (W[0]) - total % sizeof (W[0]) - 1;

    W[index] |= 0x80 << octet * 8;
    if (index >= BLOCK_SIZE - 2) {
        W[elems(W) - 2] = 0;
        W[elems(W) - 1] = 0;
        process();
        std::fill (begin (W), end (W), 0);
        index = 0;
    } else {
        ++index;
    }

    std::fill (begin (W) + index, end (W), 0);

    union {
        uint32_t full;
        uint8_t  part[4];
    } swapper;

    swapper.full = 0;
    swapper.part[3] = uint8_t(total);

    total *= 8;

    W[BLOCK_SIZE - 2] = 0x00000000;
    W[BLOCK_SIZE - 1] = total << 24;
    process ();

    state = FINISHED;
}


SHA1::digest_t
SHA1::digest (void) const {
    assert (state == FINISHED);

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
