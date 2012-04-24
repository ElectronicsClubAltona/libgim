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

#ifndef __UTIL_VECTOR_HPP
#define __UTIL_VECTOR_HPP

#include "json.hpp"

#include <iostream>

namespace util {
    struct vector {
        double x, y, z;

        vector  operator* (double) const;
        vector& operator*=(double);
        vector  operator* (const vector&) const;
        vector& operator*=(const vector&);

        vector  operator+ (const vector&) const;
        vector& operator+=(const vector&);

        vector  operator- (void) const;
        vector  operator- (const vector&) const;
        vector& operator-=(const vector&);

        vector& operator =(const vector &);

        bool operator== (const vector &) const;

        double magnitude  (void) const;
        double magnitude2 (void) const;

        double dot   (const vector&) const;
        vector cross (const vector&) const;

        vector& normalise  (void);
        vector  normalised (void) const;

        static vector spherical_to_cartesian (const vector &);
        static vector cartesian_to_spherical (const vector &);

        void sanity (void) const;
    };

    util::vector operator* (double, const util::vector&);

    std::ostream& operator<< (std::ostream&, const util::vector&);
    const json::node& operator>> (const json::node&, util::vector&);

}


#endif

