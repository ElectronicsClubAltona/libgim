/*
 * This file is part of libgim
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2014 Danny Robson <danny@nerdcruft.net>
 */

#include "sha2.hpp"

#include "bitwise.hpp"
#include "endian.hpp"

#include <algorithm>

using namespace util::hash;

// First 32 bits of the fractional parts
//               of the cube roots
//               of the first 64 prime numbers
static const
uint32_t K_64[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};


// First 64 bits of the fractional parts
//               of the cube roots
//               of the first 80 prime numbers
static const
uint64_t K_80[] = {
    0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
    0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
    0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
    0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694,
    0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
    0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
    0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4,
    0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70,
    0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
    0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
    0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30,
    0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
    0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
    0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
    0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
    0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b,
    0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
    0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
    0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
    0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817,
};


///////////////////////////////////////////////////////////////////////////////
static constexpr uint32_t
Ch (uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (~x & z);
}

static constexpr uint32_t
Maj (uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (x & z) ^ (y & z);
}


///////////////////////////////////////////////////////////////////////////////
#define FUNC(NAME,r0,r1,r2)                     \
static constexpr uint32_t                       \
NAME (uint32_t x) {                             \
    return rotater (x, (r0)) ^                  \
           rotater (x, (r1)) ^                  \
           rotater (x, (r2));                   \
}


FUNC(S0, 2, 13, 22)
FUNC(S1, 6, 11, 25)

#undef FUNC


///////////////////////////////////////////////////////////////////////////////
#define FUNC(NAME,r0,r1,s)                      \
static constexpr                                \
uint32_t NAME (uint32_t x) {                    \
    return rotater (x, (r0)) ^                  \
           rotater (x, (r1)) ^                  \
           (x >> (s));                          \
}

FUNC(s0,  7, 18,  3)
FUNC(s1, 17, 19, 10)

#undef FUNC

///////////////////////////////////////////////////////////////////////////////
static const
uint32_t H_224[] = {
    0xc1059ed8,
    0x367cd507,
    0x3070dd17,
    0xf70e5939,
    0xffc00b31,
    0x68581511,
    0x64f98fa7,
    0xbefa4fa4,
};


static const
uint32_t H_256[] = {
    0x6a09e667,
    0xbb67ae85,
    0x3c6ef372,
    0xa54ff53a,
    0x510e527f,
    0x9b05688c,
    0x1f83d9ab,
    0x5be0cd19,
};


static const
uint64_t H_384[] = {
    0xcbbb9d5dc1059ed8,
    0x629a292a367cd507,
    0x9159015a3070dd17,
    0x152fecd8f70e5939,
    0x67332667ffc00b31,
    0x8eb44a8768581511,
    0xdb0c2e0d64f98fa7,
    0x47b5481dbefa4fa4,
};


static const
uint64_t H_512[] = {
    0x6a09e667f3bcc908,
    0xbb67ae8584caa73b,
    0x3c6ef372fe94f82b,
    0xa54ff53a5f1d36f1,
    0x510e527fade682d1,
    0x9b05688c2b3e6c1f,
    0x1f83d9abfb41bd6b,
    0x5be0cd19137e2179,
};

///////////////////////////////////////////////////////////////////////////////
SHA256::SHA256 ():
    m_total (0)
{
    std::copy (std::begin (H_256), std::end (H_256), std::begin (H));
}


void
SHA256::update (const uint8_t *data, size_t length) {
    while (length) {
        size_t buffered = m_total % sizeof (M);
        size_t chunk = std::min (sizeof (M) - buffered, length);
        std::copy (data, data + chunk, C.begin () + buffered);

        length -= chunk;
        m_total  += chunk;

        if (m_total % sizeof (M) == 0)
            process ();
    }
}


void
SHA256::finish (void) {
    // Append a single 1 bit followed by 0s.
    size_t buffered = m_total % sizeof (M);
    size_t used     = m_total * 8;

    C[buffered++] = 0x80;
    ++m_total;

    // Pad out to 56 byte length
    if (buffered > 56) {
        size_t chunk = sizeof (M) - buffered;
        std::fill_n (C.begin () + buffered, chunk, 0);
        m_total += chunk;
        process ();

        buffered = 0;
    }

    size_t chunk = sizeof (M) - sizeof (uint64_t) - buffered;
    std::fill_n (C.begin () + buffered, chunk, 0);
    m_total += chunk;

    // Finish with the m_total size
    C[56] = (used >> 56) & 0xFF;
    C[57] = (used >> 48) & 0xFF;
    C[58] = (used >> 40) & 0xFF;
    C[59] = (used >> 32) & 0xFF;
    C[60] = (used >> 24) & 0xFF;
    C[61] = (used >> 16) & 0xFF;
    C[62] = (used >>  8) & 0xFF;
    C[63] = (used >>  0) & 0xFF;
    m_total += 8;

    // Reprocess
    process ();
}


void
SHA256::process (void) {
    CHECK_EQ (m_total % sizeof (M), 0);

    // Initialise the message schedule, W
    uint32_t W[64];
    std::transform (M.begin (), M.end (), std::begin (W), [] (uint32_t x) { return ntoh (x); });

    for (size_t i = 16; i < 64; ++i)
        W[i] = s1(W[i - 2]) + W[i - 7] + s0 (W[i - 15]) + W[i - 16];

    // Working variables
    uint32_t a = H[0],
             b = H[1],
             c = H[2],
             d = H[3],
             e = H[4],
             f = H[5],
             g = H[6],
             h = H[7];

    for (unsigned i = 0; i < 64; ++i) {
        uint32_t t1 = h + S1 (e) + Ch (e, f, g) + K_64[i] + W[i];
        uint32_t t2 = S0 (a) + Maj (a, b, c);

        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    H[0] += a;
    H[1] += b;
    H[2] += c;
    H[3] += d;
    H[4] += e;
    H[5] += f;
    H[6] += g;
    H[7] += h;
}


SHA256::digest_t
SHA256::digest (void) const {
    digest_t out;

    auto cursor = out.begin ();
    for (auto i: H) {
        *cursor++ = (i >> 24) & 0xFF;
        *cursor++ = (i >> 16) & 0xFF;
        *cursor++ = (i >>  8) & 0xFF;
        *cursor++ = (i >>  0) & 0xFF;
    }

    return out;
}
