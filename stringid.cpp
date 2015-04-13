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
 * Copyright 2014 Danny Robson <danny@nerdcruft.net>
 */

#include "stringid.hpp"

#include "raii.hpp"

#include <stdexcept>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>

using util::stringid;


//-----------------------------------------------------------------------------
stringid::id_t
stringid::add (const std::string key) {
    auto pos = m_map.find (key);
    if (pos != m_map.end ())
        throw std::invalid_argument ("duplicate stringid key");

    static boost::mutex s_lock;
    boost::lock_guard<boost::mutex> raii (s_lock);

    id_t id = m_map.size ();
    m_map[key] = id;
    return id;
}


//-----------------------------------------------------------------------------
stringid::id_t
stringid::find (const std::string &key) const {
    auto pos = m_map.find (key);
    if (pos == m_map.end ())
        throw std::out_of_range ("invalid stringid key");

    return pos->second;
}


//-----------------------------------------------------------------------------
void
stringid::clear (void) {
    m_map.clear ();
}
