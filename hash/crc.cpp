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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#include "./crc.hpp"

#include "../debug.hpp"

#include <array>

using util::hash::crc;


///////////////////////////////////////////////////////////////////////////////
template <typename DigestT, DigestT Generator>
const std::array<DigestT,256>
crc<DigestT,Generator>::s_table = crc<DigestT,Generator>::table ();


///////////////////////////////////////////////////////////////////////////////
template <typename DigestT, DigestT Generator>
crc<DigestT,Generator>::crc () noexcept
{
    reset ();
}


//-----------------------------------------------------------------------------
template <typename DigestT, DigestT Generator>
void
crc<DigestT,Generator>::reset (void) noexcept
{
    m_digest = 0;
    m_digest = ~m_digest;
}


///////////////////////////////////////////////////////////////////////////////
template <typename DigestT, DigestT Generator>
void
crc<DigestT,Generator>::update (const uint8_t *restrict first,
                                 const uint8_t *restrict last) noexcept
{
    CHECK_LE (first, last);

    for (auto cursor = first; cursor != last; ++cursor) {
        m_digest = s_table[*cursor ^ (m_digest & 0xFF)] ^ (m_digest >> 8u);
    }
}


//-----------------------------------------------------------------------------
template <typename DigestT, DigestT Generator>
void
crc<DigestT,Generator>::update (const void *restrict _data, size_t len) noexcept
{
    auto data = reinterpret_cast<const uint8_t *restrict> (_data);
    return update(data, data + len);
}


//-----------------------------------------------------------------------------
template <typename DigestT, DigestT Generator>
void
crc<DigestT,Generator>::finish (void)
{
    ;
}


//-----------------------------------------------------------------------------
template <typename DigestT, DigestT Generator>
DigestT
crc<DigestT,Generator>::digest (void) const
{
    return ~m_digest;
}


///////////////////////////////////////////////////////////////////////////////
template <typename DigestT, DigestT Generator>
std::array<DigestT,256>
util::hash::crc<DigestT,Generator>::table (void)
{
    auto gen = Generator;

    std::array<digest_t,256> value {};

    for (int i = 0; i < 256; ++i) {
        digest_t c = i;

        for (int k = 0; k < 8; ++k) {
            if (c & 1)
                c = gen ^ (c >> 1);
            else
                c >>= 1;
        }

        value[i] = c;
    }

    return value;
};


///////////////////////////////////////////////////////////////////////////////
template class util::hash::crc<uint32_t, 0xEDB88320>; // png
//template class util::hash::crc<uint32_t, 0X04C11DB7>; // ogg
