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

#include "md5.hpp"

#include "../bitwise.hpp"

#include <cstring>


using util::hash::MD5;


// Per-round mixing functions
static inline uint32_t
F (uint32_t x, uint32_t y, uint32_t z)
    //{ return (x & y) | (~x & z); }
    { return z ^ (x & (y ^ z)); }


static inline uint32_t
G (uint32_t x, uint32_t y, uint32_t z)
    //{ return (x & z) | (y & ~z); }
    { return F (z, x, y); }


static inline uint32_t
H (uint32_t x, uint32_t y, uint32_t z)
    { return x ^ y ^ z; }


static inline uint32_t
I (uint32_t x, uint32_t y, uint32_t z)
    { return y ^ (x | ~z); }


// Mixing constants for all rounds. They are defined as 'abs(sin(i)) * max_uint32', but we use the
// literals to avoid any stupid maths issues during compilation.
const std::array<uint32_t, 65> T = { {
    0x00000000,

    // Round 1
    0xd76aa478u, 0xe8c7b756u, 0x242070dbu, 0xc1bdceeeu,
    0xf57c0fafu, 0x4787c62au, 0xa8304613u, 0xfd469501u,
    0x698098d8u, 0x8b44f7afu, 0xffff5bb1u, 0x895cd7beu,
    0x6b901122u, 0xfd987193u, 0xa679438eu, 0x49b40821u,

    // Round 2
    0xf61e2562u, 0xc040b340u, 0x265e5a51u, 0xe9b6c7aau,
    0xd62f105du, 0x02441453u, 0xd8a1e681u, 0xe7d3fbc8u,
    0x21e1cde6u, 0xc33707d6u, 0xf4d50d87u, 0x455a14edu,
    0xa9e3e905u, 0xfcefa3f8u, 0x676f02d9u, 0x8d2a4c8au,

    // Round 3
    0xfffa3942u, 0x8771f681u, 0x6d9d6122u, 0xfde5380cu,
    0xa4beea44u, 0x4bdecfa9u, 0xf6bb4b60u, 0xbebfbc70u,
    0x289b7ec6u, 0xeaa127fau, 0xd4ef3085u, 0x04881d05u,
    0xd9d4d039u, 0xe6db99e5u, 0x1fa27cf8u, 0xc4ac5665u,

    // Round 4
    0xf4292244u, 0x432aff97u, 0xab9423a7u, 0xfc93a039u,
    0x655b59c3u, 0x8f0ccc92u, 0xffeff47du, 0x85845dd1u,
    0x6fa87e4fu, 0xfe2ce6e0u, 0xa3014314u, 0x4e0811a1u,
    0xf7537e82u, 0xbd3af235u, 0x2ad7d2bbu, 0xeb86d391u
} };



static const uint32_t DEFAULT_A = 0x67452301;
static const uint32_t DEFAULT_B = 0xefcdab89;
static const uint32_t DEFAULT_C = 0x98badcfe;
static const uint32_t DEFAULT_D = 0x10325476;


MD5::MD5()
{
    reset ();
}


void
MD5::reset (void) {
    m_total = 0;

    ABCD[0] = DEFAULT_A;
    ABCD[1] = DEFAULT_B;
    ABCD[2] = DEFAULT_C;
    ABCD[3] = DEFAULT_D;
}


void
MD5::update (const void *data, size_t len)
    { MD5::update (static_cast<const uint8_t*> (data), len); }


void
MD5::update (const uint8_t *data, size_t size) {
    size_t offset = m_total % sizeof (Xb);
    size_t remain = sizeof (Xb) - offset;

    if (size >= remain) {
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


MD5::digest_t
MD5::digest (void) const {
    static_assert (sizeof (ABCD) == sizeof (digest_t),
                   "Hash state must be the same size as the final digest");

    digest_t d;
    memcpy (d.data (), ABCD.data (), sizeof (ABCD));
    return d;
}


void
MD5::finish (void) {
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


void
MD5::transform (void) {
    uint32_t A = ABCD[0],
             B = ABCD[1],
             C = ABCD[2],
             D = ABCD[3];

    #define ROUNDx(a,b,c,d,k,s,i,f) do {            \
        (a) += (f)((b), (c), (d)) + X[k] + T[i];    \
        (a)  = rotatel ((a), (s));                  \
        (a) += (b);                                 \
    } while (0)

    // Round 1
    ROUNDx(A,B,C,D,  0,  7,  1, F);
    ROUNDx(D,A,B,C,  1, 12,  2, F);
    ROUNDx(C,D,A,B,  2, 17,  3, F);
    ROUNDx(B,C,D,A,  3, 22,  4, F);

    ROUNDx(A,B,C,D,  4,  7,  5, F);
    ROUNDx(D,A,B,C,  5, 12,  6, F);
    ROUNDx(C,D,A,B,  6, 17,  7, F);
    ROUNDx(B,C,D,A,  7, 22,  8, F);

    ROUNDx(A,B,C,D,  8,  7,  9, F);
    ROUNDx(D,A,B,C,  9, 12, 10, F);
    ROUNDx(C,D,A,B, 10, 17, 11, F);
    ROUNDx(B,C,D,A, 11, 22, 12, F);

    ROUNDx(A,B,C,D, 12,  7, 13, F);
    ROUNDx(D,A,B,C, 13, 12, 14, F);
    ROUNDx(C,D,A,B, 14, 17, 15, F);
    ROUNDx(B,C,D,A, 15, 22, 16, F);

    // Round 2
    ROUNDx(A,B,C,D,  1,  5, 17, G);
    ROUNDx(D,A,B,C,  6,  9, 18, G);
    ROUNDx(C,D,A,B, 11, 14, 19, G);
    ROUNDx(B,C,D,A,  0, 20, 20, G);

    ROUNDx(A,B,C,D,  5,  5, 21, G);
    ROUNDx(D,A,B,C, 10,  9, 22, G);
    ROUNDx(C,D,A,B, 15, 14, 23, G);
    ROUNDx(B,C,D,A,  4, 20, 24, G);

    ROUNDx(A,B,C,D,  9,  5, 25, G);
    ROUNDx(D,A,B,C, 14,  9, 26, G);
    ROUNDx(C,D,A,B,  3, 14, 27, G);
    ROUNDx(B,C,D,A,  8, 20, 28, G);

    ROUNDx(A,B,C,D, 13,  5, 29, G);
    ROUNDx(D,A,B,C,  2,  9, 30, G);
    ROUNDx(C,D,A,B,  7, 14, 31, G);
    ROUNDx(B,C,D,A, 12, 20, 32, G);

    // Round 3
    ROUNDx(A,B,C,D,  5,  4, 33, H);
    ROUNDx(D,A,B,C,  8, 11, 34, H);
    ROUNDx(C,D,A,B, 11, 16, 35, H);
    ROUNDx(B,C,D,A, 14, 23, 36, H);

    ROUNDx(A,B,C,D,  1,  4, 37, H);
    ROUNDx(D,A,B,C,  4, 11, 38, H);
    ROUNDx(C,D,A,B,  7, 16, 39, H);
    ROUNDx(B,C,D,A, 10, 23, 40, H);

    ROUNDx(A,B,C,D, 13,  4, 41, H);
    ROUNDx(D,A,B,C,  0, 11, 42, H);
    ROUNDx(C,D,A,B,  3, 16, 43, H);
    ROUNDx(B,C,D,A,  6, 23, 44, H);

    ROUNDx(A,B,C,D,  9,  4, 45, H);
    ROUNDx(D,A,B,C, 12, 11, 46, H);
    ROUNDx(C,D,A,B, 15, 16, 47, H);
    ROUNDx(B,C,D,A,  2, 23, 48, H);

    // Round 4
    ROUNDx(A,B,C,D,  0,  6, 49, I);
    ROUNDx(D,A,B,C,  7, 10, 50, I);
    ROUNDx(C,D,A,B, 14, 15, 51, I);
    ROUNDx(B,C,D,A,  5, 21, 52, I);

    ROUNDx(A,B,C,D, 12,  6, 53, I);
    ROUNDx(D,A,B,C,  3, 10, 54, I);
    ROUNDx(C,D,A,B, 10, 15, 55, I);
    ROUNDx(B,C,D,A,  1, 21, 56, I);

    ROUNDx(A,B,C,D,  8,  6, 57, I);
    ROUNDx(D,A,B,C, 15, 10, 58, I);
    ROUNDx(C,D,A,B,  6, 15, 59, I);
    ROUNDx(B,C,D,A, 13, 21, 60, I);

    ROUNDx(A,B,C,D,  4,  6, 61, I);
    ROUNDx(D,A,B,C, 11, 10, 62, I);
    ROUNDx(C,D,A,B,  2, 15, 63, I);
    ROUNDx(B,C,D,A,  9, 21, 64, I);

    ABCD[0] += A;
    ABCD[1] += B;
    ABCD[2] += C;
    ABCD[3] += D;
}
