/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
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
