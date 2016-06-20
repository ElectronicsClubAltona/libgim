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

using util::hash::crc32;


///////////////////////////////////////////////////////////////////////////////
void
crc32::reset (void)
{ ; }


///////////////////////////////////////////////////////////////////////////////
void
crc32::update (const uint8_t *restrict first,
               const uint8_t *restrict last) noexcept
{
    CHECK_LE (first, last);

    return update (first, last - first);
}


//-----------------------------------------------------------------------------
void
crc32::update (const void *restrict, size_t) noexcept
{
    not_implemented ();

    /*
    const uint8_t *restrict data = static_cast<const uint8_t*> (_data);
    static const uint32_t POLYNOMIAL = hton (static_cast<uint32_t>(0x04C11DB7));

    uint64_t bits = 0;
    unsigned int i = 0;

    if (size == 0)
        return POLYNOMIAL;

    switch (size) {
        default:    bits |= static_cast<uint64_t>(data[3]) << 32U;
        case 3:     bits |= static_cast<uint64_t>(data[2]) << 40U;
        case 2:     bits |= static_cast<uint64_t>(data[1]) << 48U;
        case 1:     bits |= static_cast<uint64_t>(data[0]) << 56U;
    }

    for (size_t i = 0; i < size; ++i) {
        for (unsigned j = 0; j < 32; ++j) {
            bool mix = bits 0x7000000000000000ULL;
            bits <<= 1;

            if (mix)
                bits ^= POLYNOMIAL << 32;
        }


    }
    */
}


//-----------------------------------------------------------------------------
void
crc32::finish (void)
{ not_implemented (); }


//-----------------------------------------------------------------------------
typename crc32::digest_t
crc32::digest (void) const
{ not_implemented (); }
