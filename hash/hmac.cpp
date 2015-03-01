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

#include "hmac.hpp"

#include "debug.hpp"

#include <algorithm>

using util::hash::HMAC;


//-----------------------------------------------------------------------------
static const uint8_t IFILL = 0x36;
static const uint8_t OFILL = 0x5C;


//-----------------------------------------------------------------------------
HMAC::HMAC (const uint8_t *restrict key, size_t len)
{
    CHECK (key);

    static_assert (sizeof (m_ikey) == sizeof (m_okey), "key padding must match");

    // If the key is larger than the blocklength, use the hash of the key
    if (len > 64) {
        m_hash.update (key, len);
        m_hash.finish ();

        auto d = m_hash.digest ();
        m_hash.reset ();

        std::copy (d.begin (), d.end (), m_ikey.begin ());
        len = d.size ();
    // Use the key directly
    } else {
        std::copy (key, key + len, m_ikey.begin ());
    }

    std::fill (m_ikey.begin () + len,
               m_ikey.end (),
               0);

    m_okey = m_ikey;

    std::transform (m_ikey.begin (),
                    m_ikey.end (),
                    m_ikey.begin (),
                    [] (auto v) { return v ^ IFILL; });

    std::transform (m_okey.begin (),
                    m_okey.end (),
                    m_okey.begin (),
                    [] (auto v) { return v ^ OFILL; });

    m_hash.update (m_ikey.data (), m_ikey.size ());
}


//-----------------------------------------------------------------------------
void
HMAC::update (const void *restrict data, size_t len)
{
    m_hash.update ((const uint8_t*)data, len);
}


//-----------------------------------------------------------------------------
void
HMAC::finish (void)
{
    m_hash.finish ();
    auto d = m_hash.digest ();

    m_hash.reset ();
    m_hash.update (m_okey.data (), m_okey.size ());
    m_hash.update (d.data (), d.size ());
    m_hash.finish ();
}


//-----------------------------------------------------------------------------
void
HMAC::reset (void)
{
    m_hash.reset ();
    m_hash.update (m_ikey.data (), m_ikey.size ());
}


//-----------------------------------------------------------------------------
HMAC::digest_t
HMAC::digest (void)
{
    return m_hash.digest ();
}
