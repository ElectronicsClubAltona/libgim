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

#include "hmac.hpp"

#include "../debug.hpp"

#include <algorithm>

using util::hash::HMAC;


//-----------------------------------------------------------------------------
static const uint8_t IFILL = 0x36;
static const uint8_t OFILL = 0x5C;


//-----------------------------------------------------------------------------
template <class T>
HMAC<T>::HMAC (const uint8_t *restrict key, size_t len)
{
    CHECK (key);

    static_assert (sizeof (m_ikey) == sizeof (m_okey), "key padding must match");

    // If the key is larger than the blocklength, use the hash of the key
    if (len > T::BLOCK_SIZE) {
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
template <class T>
void
HMAC<T>::update (const void *restrict data, size_t len)
{
    m_hash.update ((const uint8_t*)data, len);
}


//-----------------------------------------------------------------------------
template <class T>
void
HMAC<T>::finish (void)
{
    m_hash.finish ();
    auto d = m_hash.digest ();

    m_hash.reset ();
    m_hash.update (m_okey.data (), m_okey.size ());
    m_hash.update (d.data (), d.size ());
    m_hash.finish ();
}


//-----------------------------------------------------------------------------
template <class T>
void
HMAC<T>::reset (void)
{
    m_hash.reset ();
    m_hash.update (m_ikey.data (), m_ikey.size ());
}


//-----------------------------------------------------------------------------
template <class T>
typename HMAC<T>::digest_t
HMAC<T>::digest (void)
{
    return m_hash.digest ();
}


//-----------------------------------------------------------------------------
#include "md5.hpp"
#include "sha1.hpp"

namespace util::hash {
    template class HMAC<MD5>;
    template class HMAC<SHA1>;
}
