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
 * Copyright 2010 Danny Robson <danny@blubinc.net>
 */


#ifndef __UTIL_REGION_HPP
#define __UTIL_REGION_HPP

#include "point.hpp"

namespace util {
    /**
     * A two-dimensional rectangle, with size and position.
     */
    template <typename T>
    struct region {
        T x, y;
        T w, h;

        region (T _x, T _y, T _w, T _h);

        region& operator +=(const vector& rhs);

        T area     (void) const;
        T diameter (void) const;

        bool empty (void) const;

        point base   (void) const;
        point centre (void) const;

        bool includes (const point&) const; // inclusive of borders
        bool contains (const point&) const; // exclusive of borders
        bool overlaps (const region<T>&) const;

        bool operator ==(const region<T>& rhs) const;
        bool operator !=(const region<T>& rhs) const
            { return !(*this == rhs); }

        void sanity (void) const;
    };
}

template <typename T>
std::ostream&
operator<< (std::ostream&, const util::region<T>&);

#endif 
