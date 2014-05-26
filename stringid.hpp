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

#ifndef __UTIL_STRINGID_HPP
#define __UTIL_STRINGID_HPP

#include <map>
#include <string>

namespace util {
    class stringid {
        public:
            typedef unsigned id_t;

            id_t add  (std::string);
            id_t find (const std::string&) const;

            void clear (void);

        private:
            std::map<const std::string, id_t> m_map;
    };
}

#endif
