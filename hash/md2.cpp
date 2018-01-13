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

#include "md2.hpp"

#include "../debug.hpp"
#include "../types.hpp"

#include <array>
#include <algorithm>
#include <cstring>


///////////////////////////////////////////////////////////////////////////////
using util::hash::MD2;


///////////////////////////////////////////////////////////////////////////////
static constexpr std::array<uint8_t,256> S = { {
     41,  46,  67, 201, 162, 216, 124,   1,  61,  54,  84, 161, 236, 240,   6,  19,
     98, 167,   5, 243, 192, 199, 115, 140, 152, 147,  43, 217, 188,  76, 130, 202,
     30, 155,  87,  60, 253, 212, 224,  22, 103,  66, 111,  24, 138,  23, 229,  18,
    190,  78, 196, 214, 218, 158, 222,  73, 160, 251, 245, 142, 187,  47, 238, 122,
    169, 104, 121, 145,  21, 178,   7,  63, 148, 194,  16, 137,  11,  34,  95,  33,
    128, 127,  93, 154,  90, 144,  50,  39,  53,  62, 204, 231, 191, 247, 151,   3,
    255,  25,  48, 179,  72, 165, 181, 209, 215,  94, 146,  42, 172,  86, 170, 198,
     79, 184,  56, 210, 150, 164, 125, 182, 118, 252, 107, 226, 156, 116,   4, 241,
     69, 157, 112,  89, 100, 113, 135,  32, 134,  91, 207, 101, 230,  45, 168,   2,
     27,  96,  37, 173, 174, 176, 185, 246,  28,  70,  97, 105,  52,  64, 126,  15,
     85,  71, 163,  35, 221,  81, 175,  58, 195,  92, 249, 206, 186, 197, 234,  38,
     44,  83,  13, 110, 133,  40, 132,   9, 211, 223, 205, 244,  65, 129,  77,  82,
    106, 220,  55, 200, 108, 193, 171, 250,  36, 225, 123,   8,  12, 189, 177,  74,
    120, 136, 149, 139, 227,  99, 232, 109, 233, 203, 213, 254,  59,   0,  29,  57,
    242, 239, 183,  14, 102,  88, 208, 228, 166, 119, 114, 248, 235, 117,  75,  10,
     49,  68,  80, 180, 143, 237,  31,  26, 219, 153, 141,  51, 159,  17, 131,  20,
} };


//-----------------------------------------------------------------------------
static const size_t M_OFFSET = 16;
static const size_t M_LENGTH = 16;


///////////////////////////////////////////////////////////////////////////////
static void
transform (std::array<uint8_t,16> &C, std::array<uint8_t,48> &X) noexcept
{
    util::view M { X.data () + M_OFFSET, M_LENGTH };

    // Update the checksum.
    // XXX: I can't see why we need the xor-assign from the spec, but it's the
    // only change keeping us from conforming to the test-cases. Pulled from a
    // review of the reference implementation.
    uint8_t L = C[15];
    for (size_t i = 0; i < std::size (C); ++i)
        L = C[i] ^= S[M[i] ^ L];


    // Setup the blocks
    for (size_t i = 0; i < 16; ++i)
        X[32 + i] = X[16 + i] ^ X[i];

    // Perform the processing rounds
    for (size_t i = 0, t = 0; i < 18; ++i) {
        for (size_t j = 0; j < 48; ++j)
            t = X[j] ^= S[t];

        t = (t + i) % 256;
    }
}


///////////////////////////////////////////////////////////////////////////////
MD2::digest_t
MD2::operator() (const util::view<const uint8_t*> data) const noexcept
{
    // zero initialise the state vectors, and create a simple window `M' into
    // the middle of the `X' state vector.
    std::array<uint8_t,16> C {};
    std::array<uint8_t,48> X {};
    const util::view M { std::begin (X) + M_OFFSET, M_LENGTH };

    // process each complete block by copying to the window `M' and
    // transforming X and C.
    //
    // leave the remainder of the data in `M' for subsequent padding.
    auto remain = data;
    while (remain.size () >= M_LENGTH) {
        std::copy_n (std::begin (remain), M_LENGTH, std::begin (M));
        transform (C, X);

        remain = { remain.begin () + M_LENGTH, remain.end () };
    };

    // Copying the remaining data then append the padding bytes. Padding
    // _must_ be performed even if we have an evenly divisible input buffer.
    auto tail = std::copy (remain.begin (), remain.end (), std::begin (M));
    auto unused = std::distance (tail, std::end (M));
    std::fill (tail, std::end (M), unused);
    transform (C, X);

    // Append the checksum and transform once more.
    std::copy (std::begin (C), std::end (C), std::begin (M));
    transform (C, X);

    // The final digest is the first `n' bytes of the state vector.
    digest_t d;
    std::copy_n (std::begin (X), d.size (), std::begin (d));
    return d;
}
