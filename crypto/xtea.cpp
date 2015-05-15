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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#include "xtea.hpp"

using util::crypto::XTEA;


///////////////////////////////////////////////////////////////////////////////
static const uint32_t MAGIC = 0x9E3779B9;

// each iteration performs two feistel rounds, for a total of 64
static const unsigned ITERATIONS = 32;


///////////////////////////////////////////////////////////////////////////////
XTEA::XTEA (key_t _key):
    m_key (_key)
{ }


//-----------------------------------------------------------------------------
void
XTEA::encrypt (uint32_t *restrict data, size_t count)
{
    if (count % 2)
        throw std::invalid_argument ("XTEA requires even data count");

    auto last = data + count;

    while (data < last) {
        uint32_t sum = 0;
        uint32_t v0 = data[0];
        uint32_t v1 = data[1];

        for (unsigned i = 0; i < ITERATIONS; ++i) {
            v0  += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + m_key[sum & 3]);
            sum += MAGIC;
            v1  += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + m_key[(sum >> 11) & 3]);
        }

        *data++ = v0;
        *data++ = v1;
    }
}


//-----------------------------------------------------------------------------
void
XTEA::decrypt (uint32_t *restrict data, size_t count)
{
    if (count % 2)
        throw std::invalid_argument ("XTEA requires even data count");

    auto last = data + count;

    while (data < last) {
        uint32_t sum = ITERATIONS * MAGIC;
        uint32_t v0 = data[0];
        uint32_t v1 = data[1];

        for (unsigned i = 0; i < ITERATIONS; ++i) {
            v1  -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + m_key[(sum >> 11) & 3]);
            sum -= MAGIC;
            v0  -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + m_key[sum & 3]);
        }

        *data++ = v0;
        *data++ = v1;
    }
}
