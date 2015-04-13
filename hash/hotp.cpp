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

#include "hotp.hpp"

#include "../endian.hpp"

#include <cstring>

using util::hash::HOTP;


//-----------------------------------------------------------------------------
HOTP::HOTP (const char *_key, uint64_t _counter):
    HOTP (_key, strlen (_key), _counter)
{ ; }


//-----------------------------------------------------------------------------
HOTP::HOTP (const void *_key, size_t _len, uint64_t _counter):
    m_counter (_counter),
    m_hash ((const uint8_t*)_key, _len)
{ ; }


//-----------------------------------------------------------------------------
unsigned
HOTP::value (void)
{
    auto c = htob (m_counter);

    m_hash.update (&c, sizeof (c));
    m_hash.finish ();

    auto h = m_hash.digest ();
    auto t = truncate (h);

    m_hash.reset ();
    ++m_counter;

    return t % 1000000;
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
