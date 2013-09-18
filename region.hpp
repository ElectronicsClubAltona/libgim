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
 * Copyright 2010 Danny Robson <danny@nerdcruft.net>
 */


#ifndef __UTIL_REGION_HPP
#define __UTIL_REGION_HPP

#include "point.hpp"
#include "types/traits.hpp"

namespace util {
    /**
     * A two-dimensional rectangle, with size and position.
     */
    template <typename T>
    struct region {
        typedef T position_type;
#if 0
        typedef typename always_unsigned<T>::type size_type;
#else
        typedef T size_type;
#endif

        T x, y;                 
        size_type w, h;

        region (T _x, T _y, size_type _w, size_type _h);

        region& operator +=(const vector<2>& rhs);

        size_type area     (void) const;
        size_type diameter (void) const;
        void scale (double factor);

        bool empty (void) const;

        point2 base    (void) const;
        point2 centre  (void) const;
        point2 closest (point2) const;

        bool includes (const point2&) const; // inclusive of borders
        bool contains (const point2&) const; // exclusive of borders
        bool overlaps (const region<T>&) const; // exclusive of borders

        void constrain (point2&) const;
        point2 constrained (const point2&) const;

        region overlap (const region<T>&) const;

        bool operator ==(const region<T>& rhs) const;
        bool operator !=(const region<T>& rhs) const
            { return !(*this == rhs); }

        static const region<T> MAX;
        static const region<T> UNIT;

        void sanity (void) const;

#if 0
        struct iterator {
            T x, y;
            T a;
            size_type w, h;

            iterator& operator++ (void);
            iterator& operator*  (void);
            bool operator== (const iterator&) const;
            bool operator!= (const iterator&) const;
        };

        iterator begin (void);
        iterator end   (void);
#endif
    };


    template <typename T>
    std::ostream& operator<< (std::ostream&, const util::region<T>&);
}


#endif 
