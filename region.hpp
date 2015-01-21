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
 * Copyright 2010-2015 Danny Robson <danny@nerdcruft.net>
 */


#ifndef __UTIL_REGION_HPP
#define __UTIL_REGION_HPP

#include "extent.hpp"
#include "point.hpp"
#include "types/traits.hpp"

namespace util {
    /**
     * A two-dimensional rectangle, with size and position.
     */
    template <typename T>
    struct region {
        typedef T position_type;
        typedef T size_type;

        T x, y;
        size_type w, h;

        region (util::point<2,T>, util::extent<size_type>);
        region (T _x, T _y, size_type _w, size_type _h);

        size_type area     (void) const;
        size_type diameter (void) const;
        void scale (T factor);

        bool empty (void) const;

        point<2,T> base    (void) const;
        point<2,T> centre  (void) const;
        point<2,T> closest (point<2,T>) const;

        // Point and region relation queries
        bool includes (const point<2,T>&) const; // inclusive of borders
        bool contains (const point<2,T>&) const; // exclusive of borders
        bool intersects (const region<T>&) const;  // exclusive of borders

        // Move a point to be within the region bounds
        void constrain (point<2,T>&) const;
        point<2,T> constrained (const point<2,T>&) const;

        // Compute binary region combinations
        region intersection (const region<T>&) const;

        // Compute a region `mag` units into the region
        region inset (T mag);

        // Logical comparison operators
        bool operator ==(const region<T>& rhs) const;
        bool operator !=(const region<T>& rhs) const
            { return !(*this == rhs); }

        // Utility constants
        static const region<T> MAX;
        static const region<T> UNIT;

        void sanity (void) const;
    };

    typedef region<size_t> region2u;
    typedef region<float>  region2f;

    template <typename T>
    std::ostream& operator<< (std::ostream&, const util::region<T>&);
}


#endif
