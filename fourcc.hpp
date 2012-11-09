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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_FOURCC_HPP
#define __UTIL_FOURCC_HPP

#include <iostream>
#include <cstdint>

namespace util {
    struct fourcc {
        uint8_t data[4];

        static fourcc from_string(const char[4]);
        static fourcc from_chars(uint8_t, uint8_t, uint8_t, uint8_t);

        bool operator== (const char[4]) const;
        operator uint32_t (void) const;
    };
}

std::ostream& operator<< (std::ostream&, util::fourcc);

#endif
