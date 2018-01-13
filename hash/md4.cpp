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

#include "../bitwise.hpp"
#include "../debug.hpp"

#include <cstring>


using util::hash::MD4;


///////////////////////////////////////////////////////////////////////////////
// Auxiliary functions for each set of rounds
static inline constexpr
uint32_t
F (uint32_t X, uint32_t Y, uint32_t Z)
{
    return (X & Y) | (~X & Z);
}


//-----------------------------------------------------------------------------
static inline constexpr
uint32_t
G (uint32_t X, uint32_t Y, uint32_t Z)
{
    return (X & Y) | (X & Z) | (Y & Z);
}


//-----------------------------------------------------------------------------
static inline constexpr
uint32_t
H (uint32_t X, uint32_t Y, uint32_t Z)
{
    return X ^ Y ^ Z;
}


//-----------------------------------------------------------------------------
static constexpr uint32_t INITIAL_A = 0x67452301;
static constexpr uint32_t INITIAL_B = 0xefcdab89;
static constexpr uint32_t INITIAL_C = 0x98badcfe;
static constexpr uint32_t INITIAL_D = 0x10325476;


///////////////////////////////////////////////////////////////////////////////
static void
transform (std::array<uint32_t,4> &ABCD, const std::array<uint32_t,16> &X) noexcept
{
    uint32_t A = ABCD[0],
        B = ABCD[1],
        C = ABCD[2],
        D = ABCD[3];

#define ROUND1(a,b,c,d,k,s) do {                \
        (a) += F((b), (c), (d)) + X[k];             \
        (a)  = util::rotatel ((a), (s));                  \
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
        (a)  = util::rotatel ((a), (s));                  \
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
        (a)  = util::rotatel ((a), (s));                  \
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


///////////////////////////////////////////////////////////////////////////////
MD4::digest_t
MD4::operator() (util::view<const uint8_t*> data) noexcept
{
    /* RESET */
    uint64_t total = 0;

    std::array<uint32_t,4> ABCD {
        INITIAL_A,
        INITIAL_B,
        INITIAL_C,
        INITIAL_D,
    };

    union {
        std::array<uint32_t,16> X;
        std::array<uint8_t, 64> Xb;
    };
    static_assert (sizeof (X) == sizeof (Xb));
    static_assert (sizeof (ABCD) == sizeof (digest_t));

    std::fill (std::begin (X), std::end (X), 0);

    /* UPDATE */
    {
        auto remain = data;
        while (remain.size () >= sizeof (Xb)) {
            std::copy_n (std::begin (remain), sizeof (Xb), std::begin (Xb));
            transform (ABCD, X);
            remain = { remain.begin () + sizeof (Xb), remain.end () };
            total += sizeof (Xb);
        }

        std::copy (std::begin (remain), std::end (remain), std::begin (Xb));
        total += remain.size ();
    }

    uint64_t bits = total * 8;

    /* FINISH */
    {
        // Pad with the mandatory 1 bit
        size_t offset = total % sizeof (Xb);
        Xb[offset] = 0x80;
    }

    {
        // Pad the remainder with 0's, until 56 bytes
        size_t offset = (total + 1) % sizeof (Xb);
        size_t remain = (56 - offset % sizeof (Xb)) % sizeof (Xb);

        if (offset > 56) {
            std::fill_n (std::begin (Xb) + offset, sizeof (Xb) - offset, 0);
            transform (ABCD, X);
            remain -= sizeof (Xb) - offset;
            offset  = 0;
        }

        std::fill (std::begin (Xb) + offset, std::end (Xb), 0);

        // Put in the length (in bits) least significant first
        for (size_t i = 0; i < sizeof (bits); ++i) {
            Xb[56 + i] = bits & 0xFF;
            bits >>= 8;
        }

        transform (ABCD, X);
    }

    /* DIGEEST */
    digest_t d;
    memcpy (d.data (), ABCD.data(), sizeof (ABCD));
    return d;
}
