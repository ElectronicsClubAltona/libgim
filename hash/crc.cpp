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

using util::hash::crc32;


///////////////////////////////////////////////////////////////////////////////
static
std::array<uint32_t,256>
make_table (void)
{
    std::array<uint32_t,256> value {};

    for (int i = 0; i < 256; ++i) {
        std::uint32_t c = i;

        for (int k = 0; k < 8; ++k) {
            if (c & 1)
                c = 0xEDB88320L ^ (c >> 1);
            else
                c >>= 1;
        }

        value[i] = c;
    }

    return value;
};


//-----------------------------------------------------------------------------
static std::array<uint32_t,256> table = make_table ();


///////////////////////////////////////////////////////////////////////////////
crc32::crc32 () noexcept
{
    reset ();
}


//-----------------------------------------------------------------------------
void
crc32::reset (void) noexcept
{
    m_digest = 0;
    m_digest = ~m_digest;
}


///////////////////////////////////////////////////////////////////////////////
void
crc32::update (const uint8_t *restrict first,
               const uint8_t *restrict last) noexcept
{
    CHECK_LE (first, last);

    for (auto cursor = first; cursor != last; ++cursor) {
        m_digest = table[*cursor ^ (m_digest & 0xFF)] ^ (m_digest >> 8u);
    }
}


//-----------------------------------------------------------------------------
void
crc32::update (const void *restrict _data, size_t len) noexcept
{
    auto data = reinterpret_cast<const uint8_t *restrict> (_data);
    return update(data, data + len);
}


//-----------------------------------------------------------------------------
void
crc32::finish (void)
{
    ;
}


//-----------------------------------------------------------------------------
typename crc32::digest_t
crc32::digest (void) const
{
    return ~m_digest;
}