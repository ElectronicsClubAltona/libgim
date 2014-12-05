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
