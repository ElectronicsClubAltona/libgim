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

#include "tea.hpp"

#include <cstdint>
#include <stdexcept>

using util::crypto::TEA;


///////////////////////////////////////////////////////////////////////////////
static const std::uint32_t MAGIC = 0x9E3779B9;

// each iteration performs two feistel rounds, for a total of 64
static const unsigned ITERATIONS = 32;


///////////////////////////////////////////////////////////////////////////////
TEA::TEA (key_t _key):
    m_key (_key)
{ ; }


//-----------------------------------------------------------------------------
void
TEA::encrypt (uint32_t *restrict dst, const uint32_t *restrict src, size_t count)
{
    if (count % 2)
        throw std::invalid_argument ("TEA requires even data count");

    auto last = src + count;
    while (src < last) {
        uint32_t sum = 0;
        uint32_t v0 = src[0];
        uint32_t v1 = src[1];

        for (unsigned i = 0; i < ITERATIONS; ++i) {
            sum += MAGIC;
            v0  += ((v1 << 4) + m_key[0]) ^ (v1 + sum) ^ ((v1 >> 5) + m_key[1]);
            v1  += ((v0 << 4) + m_key[2]) ^ (v0 + sum) ^ ((v0 >> 5) + m_key[3]);
        }

        dst[0] = v0;
        dst[1] = v1;

        src += 2;
        dst += 2;
    }
}


//-----------------------------------------------------------------------------
void
TEA::decrypt (uint32_t *restrict dst, const uint32_t *restrict src, size_t count)
{
    if (count % 2)
        throw std::invalid_argument ("TEA requires even data count");

    auto last = src + count;

    while (src < last) {
        uint32_t sum = MAGIC << 5;
        uint32_t v0 = src[0];
        uint32_t v1 = src[1];

        for (unsigned i = 0; i < ITERATIONS; ++i) {
            v1  -= ((v0 << 4) + m_key[2]) ^ (v0 + sum) ^ ((v0 >> 5) + m_key[3]);
            v0  -= ((v1 << 4) + m_key[0]) ^ (v1 + sum) ^ ((v1 >> 5) + m_key[1]);
            sum -= MAGIC;
        }

        dst[0] = v0;
        dst[1] = v1;

        src += 2;
        dst += 2;
    }
}
