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
 * Copyright 2012 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_LOG_IPP
#define __UTIL_LOG_IPP

#include <boost/format.hpp>

namespace util {
    namespace detail  {
        void
        log (level_t l, boost::format &&format);

        template <typename T, typename ...tail>
        void
        log (level_t l, boost::format &&format, const T &val, tail ..._tail) {
            ::util::detail::log (l, std::move (format % val), _tail...);
        }
    }

    template <typename ...tail>
    void log (level_t l, const std::string &format, tail ..._tail)
        { detail::log (l, std::move (boost::format (format)), _tail...); }
}


#endif
