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
 * Copyright 2015-2018 Danny Robson <danny@nerdcruft.net>
 */

#include "hotp.hpp"

#include "../endian.hpp"

#include <cstring>

using util::hash::HOTP;


///////////////////////////////////////////////////////////////////////////////
HOTP::HOTP (util::view<const char*> _key, uint64_t _counter):
    m_counter (_counter),
    m_hash (_key.template cast<const uint8_t> ())
{ ; }



//-----------------------------------------------------------------------------
unsigned
HOTP::value (void)
{
    union {
        uint64_t number;
        uint8_t  bytes[8];
    };

    number = htob (m_counter);

    auto res = truncate (m_hash (util::make_cview (bytes)));
    ++m_counter;
    return res % 1'000'000;
}


//-----------------------------------------------------------------------------
uint32_t
HOTP::truncate (SHA1::digest_t d)
{
    // offset into the digest by the last 4 bits
    size_t o= d[d.size () - 1] & 0x0F;

    // mask the highest bit per the specification
    uint32_t v = (d[o + 0] & 0x7f) << 24 |
                 (d[o + 1] & 0xff) << 16 |
                 (d[o + 2] & 0xff) <<  8 |
                 (d[o + 3] & 0xff) <<  0;

    return v;
}
