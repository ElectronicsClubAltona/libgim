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

#include "md4.hpp"

#include "bitwise.hpp"
#include "types.hpp"
#include "endian.hpp"

#include <cstring>


using util::hash::MD4;
using std::array;


// Auxiliary functions for each set of rounds
static inline uint32_t
F (uint32_t X, uint32_t Y, uint32_t Z)
    { return (X & Y) | (~X & Z); }

static inline uint32_t
G (uint32_t X, uint32_t Y, uint32_t Z)
    { return (X & Y) | (X & Z) | (Y & Z); }

static inline uint32_t
H (uint32_t X, uint32_t Y, uint32_t Z)
    { return X ^ Y ^ Z; }


// Constructors and setup functions
static const uint32_t DEFAULT_A = 0x67452301;
static const uint32_t DEFAULT_B = 0xefcdab89;
static const uint32_t DEFAULT_C = 0x98badcfe;
static const uint32_t DEFAULT_D = 0x10325476;


MD4::MD4 ()
{
    reset ();
    static_assert (sizeof (MD4::X) == sizeof (MD4::Xb),
                   "Byte and word buffer size must match exactly");
    static_assert (sizeof (MD4::ABCD) == sizeof (MD4::digest_t),
                   "Internal state must match the size of the digest");
}


void
MD4::reset (void) {
    m_total = 0;

    ABCD[0] = DEFAULT_A;
    ABCD[1] = DEFAULT_B;
    ABCD[2] = DEFAULT_C;
    ABCD[3] = DEFAULT_D;

    memset (Xb, 0, sizeof (Xb));
}


void
MD4::update (const void *data, size_t size)
    { update (static_cast<const uint8_t*> (data), size); }


void
MD4::update (const uint8_t *data, size_t size) {
    size_t offset = m_total % sizeof (Xb);
    size_t remain = sizeof (Xb) - offset;

    if (size > remain) {
        memcpy (Xb + offset, data, remain);
        transform ();

        m_total += remain;
        size    -= remain;
        data    += remain;

        while (size >= sizeof (Xb)) {
            memcpy (Xb, data, sizeof (Xb));
            transform ();

            m_total += sizeof (Xb);
            size    -= sizeof (Xb);
            data    += sizeof (Xb);
        }

        offset = 0;
    }

    memcpy (Xb + offset, data, size);
    m_total += size;
}


MD4::digest_t
MD4::digest (void) const {
    digest_t d;
    memcpy (d.data (), ABCD.data(), sizeof (ABCD));
    return d; 
}


void
MD4::transform (void) {
    uint32_t A = ABCD[0],
             B = ABCD[1],
             C = ABCD[2],
             D = ABCD[3];

    #define ROUND1(a,b,c,d,k,s) do {                \
        (a) += F((b), (c), (d)) + X[k];             \
        (a)  = rotatel ((a), (s));                  \
    } while (0)

    ROUND1(A,B,C,D,  0,  3);
    ROUND1(D,A,B,C,  1,  7);
    ROUND1(C,D,A,B,  2, 11);
    ROUND1(B,C,D,A,  3, 19);
    
    ROUND1(A,B,C,D,  4,  3);
    ROUND1(D,A,B,C,  5,  7);
    ROUND1(C,D,A,B,  6, 11);
    ROUND1(B,C,D,A,  7, 19);
    
    ROUND1(A,B,C,D,  8,  3);
    ROUND1(D,A,B,C,  9,  7);
    ROUND1(C,D,A,B, 10, 11);
    ROUND1(B,C,D,A, 11, 19);
    
    ROUND1(A,B,C,D, 12,  3);
    ROUND1(D,A,B,C, 13,  7);
    ROUND1(C,D,A,B, 14, 11);
    ROUND1(B,C,D,A, 15, 19);

    #define ROUND2(a,b,c,d,k,s) do {                \
        (a) += G((b),(c),(d)) + X[k] + 0x5A827999u; \
        (a)  = rotatel ((a), (s));                  \
    } while (0)

    ROUND2(A,B,C,D,  0,  3);
    ROUND2(D,A,B,C,  4,  5);
    ROUND2(C,D,A,B,  8,  9);
    ROUND2(B,C,D,A, 12, 13);

    ROUND2(A,B,C,D,  1,  3);
    ROUND2(D,A,B,C,  5,  5);
    ROUND2(C,D,A,B,  9,  9);
    ROUND2(B,C,D,A, 13, 13);

    ROUND2(A,B,C,D,  2,  3);
    ROUND2(D,A,B,C,  6,  5);
    ROUND2(C,D,A,B, 10,  9);
    ROUND2(B,C,D,A, 14, 13);

    ROUND2(A,B,C,D,  3,  3);
    ROUND2(D,A,B,C,  7,  5);
    ROUND2(C,D,A,B, 11,  9);
    ROUND2(B,C,D,A, 15, 13);

    #define ROUND3(a,b,c,d,k,s) do {                \
        (a) += H((b),(c),(d)) + X[k] + 0x6ED9EBA1u; \
        (a)  = rotatel ((a), (s));                  \
    } while (0)

    ROUND3(A,B,C,D,  0,  3);
    ROUND3(D,A,B,C,  8,  9);
    ROUND3(C,D,A,B,  4, 11);
    ROUND3(B,C,D,A, 12, 15);

    ROUND3(A,B,C,D,  2,  3);
    ROUND3(D,A,B,C, 10,  9);
    ROUND3(C,D,A,B,  6, 11);
    ROUND3(B,C,D,A, 14, 15);

    ROUND3(A,B,C,D,  1,  3);
    ROUND3(D,A,B,C,  9,  9);
    ROUND3(C,D,A,B,  5, 11);
    ROUND3(B,C,D,A, 13, 15);

    ROUND3(A,B,C,D,  3,  3);
    ROUND3(D,A,B,C, 11,  9);
    ROUND3(C,D,A,B,  7, 11);
    ROUND3(B,C,D,A, 15, 15);

    ABCD[0] += A;
    ABCD[1] += B;
    ABCD[2] += C;
    ABCD[3] += D;
}


void
MD4::finish (void) {
    uint64_t bits = m_total * 8;

    {
        // Pad with the mandatory 1 bit
        size_t offset = m_total % sizeof (Xb);
        Xb[offset] = 0x80;
    }

    {
        // Pad the remainder with 0's, until 56 bytes
        size_t offset = (m_total + 1) % sizeof (Xb);
        size_t remain = (56 - offset % sizeof (Xb)) % sizeof (Xb);

        if (offset > 56) {
            memset (Xb + offset, 0, sizeof (Xb) - offset);
            transform ();
            remain -= sizeof (Xb) - offset;
            offset  = 0;
        }

        memset (Xb + offset, 0, remain);

        // Put in the length (in bits) least significant first
        for (size_t i = 0; i < sizeof (bits); ++i) {
            Xb[56 + i] = bits & 0xFF;
            bits >>= 8;
        }

        transform ();
    }
}

