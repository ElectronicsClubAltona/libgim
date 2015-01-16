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
#if 0
        typedef typename always_unsigned<T>::type size_type;
#else
        typedef T size_type;
#endif

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

        bool includes (const point<2,T>&) const; // inclusive of borders
        bool contains (const point<2,T>&) const; // exclusive of borders
        bool overlaps (const region<T>&) const; // exclusive of borders

        void constrain (point<2,T>&) const;
        point<2,T> constrained (const point<2,T>&) const;

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

    typedef region<size_t> region2u;

    template <typename T>
    std::ostream& operator<< (std::ostream&, const util::region<T>&);
}


#endif
