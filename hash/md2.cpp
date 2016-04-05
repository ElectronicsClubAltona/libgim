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

#include "../types.hpp"

#include <array>
#include <algorithm>
#include <cstring>


///////////////////////////////////////////////////////////////////////////////
using util::hash::MD2;

using std::array;
using std::begin;
using std::end;
using std::fill;


///////////////////////////////////////////////////////////////////////////////
static array<uint8_t,256> S = { {
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


///////////////////////////////////////////////////////////////////////////////
MD2::MD2 ()
{
    reset ();
}


//-----------------------------------------------------------------------------
void
MD2::reset (void) {
    m_total = 0;
    fill (begin (C), end (C), 0);
    fill (begin (X), end (X), 0);
}


//-----------------------------------------------------------------------------
void
MD2::update (const void *data, size_t size)
    { update (static_cast<const uint8_t*> (data), size); }


//-----------------------------------------------------------------------------
static const size_t M_OFFSET = 16;
static const size_t M_LENGTH = 16;


//-----------------------------------------------------------------------------
void
MD2::update (const uint8_t *data, size_t size) {
    uint8_t *M = X + M_OFFSET;
    size_t offset = m_total % M_LENGTH;
    size_t remain = M_LENGTH - offset;

    if (size > remain) {
        memcpy (M + offset, data, remain);
        transform ();

        m_total += remain;
        size    -= remain;
        data    += remain;

        while (size >= M_LENGTH) {
            memcpy (M, data, M_LENGTH);
            transform ();

            m_total += M_LENGTH;
            size    -= M_LENGTH;
            data    += M_LENGTH;
        }

        offset = 0;
    }

    memcpy (M + offset, data, size);
    m_total += size;
}


//-----------------------------------------------------------------------------
MD2::digest_t
MD2::digest (void) const {
    digest_t d;
    memcpy (d.data (), X, sizeof (d));
    return d;
}


//-----------------------------------------------------------------------------
void
MD2::transform (void) {
    uint8_t *M = X + M_OFFSET;

    // Update the checksum.
    // XXX: I can't see why we need the xor-assign from the spec, but it's the
    // only change keeping us from conforming to the test-cases. Pulled from a
    // review of the reference implementation.
    uint8_t L = C[15];
    for (size_t i = 0; i < elems (C); ++i)
        L = C[i] ^= S[M[i] ^ L];

    uint8_t t = 0;

    // Setup the blocks
    for (size_t i = 0; i < 16; ++i)
       X[32 + i] = X[16 + i] ^ X[i];

    // Perform the processing rounds
    for (size_t i = 0; i < 18; ++i) {
        for (size_t j = 0; j < 48; ++j)
            t = X[j] = X[j] ^ S[t];

        t = (t + i) % 256;
    }
}


void
MD2::finish (void) {
    uint8_t *M = X + M_OFFSET;

    // Append the padding bytes
    size_t offset = m_total % M_LENGTH;
    size_t remain = M_LENGTH - offset;

    for (size_t i = 0; i < remain; ++i)
        M[offset + i] = remain;
    transform ();

    // Append the checksum
    memcpy (M, C, sizeof (C));
    transform ();
}

