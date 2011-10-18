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

#ifndef __UTIL_POINT_HPP
#define __UTIL_POINT_HPP

#include <iostream>

#include "vector.hpp"

namespace util {
    /// A three dimensional position in space.
    struct point {
        double x, y, z;

        double distance  (const point &) const;
        double manhattan (const point &) const;

        point& operator*= (double);
        point  operator*  (double) const;

        point  operator+  (const vector&) const;
        point& operator+= (const vector&);
        vector operator-  (const point&) const;

        void sanity (void) const;
    };
}

std::ostream& operator<< (std::ostream&, const util::point&);


#endif // __UTIL_POINT_HPP
