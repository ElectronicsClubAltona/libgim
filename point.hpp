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
 * Copyright 2011-14 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_POINT_HPP
#define __UTIL_POINT_HPP

#include "vector.hpp"
#include "detail/coord.hpp"

#include <array>
#include <initializer_list>
#include <iostream>

namespace util {
    /// An n-dimensional position in space.
    template <size_t S, typename T>
    struct point : public detail::coord<S,T> {
        static_assert (S > 0, "point dimensions must be strictly positive.");

        point ();

        template <typename... U>
        point (U ...u): detail::coord<S,T> {std::forward<U> (u)...} { ; }

        // point operators
        T distance  (const point &) const;
        T distance2 (const point &) const;
        T manhattan (const point &) const;

        vector<S,T> to (const point&) const;

        // arithetic operators
        point<S,T>& operator*= (T);
        point<S,T>  operator*  (T) const;
        point<S,T>  operator-  (const point<S,T>&) const;

        point<S,T>  operator-  (const vector<S,T>&) const;
        point<S,T>& operator-= (const vector<S,T>&);
        point<S,T>  operator+  (const vector<S,T>&) const;
        point<S,T>& operator+= (const vector<S,T>&);

        template <size_t D> point<D,T> redim (void) const;

        void sanity (void) const;
    };

    // free maths operators
    template <size_t S, typename T> point<S,T> operator* (const vector<S,T>&, const point<S,T>&);
    template <size_t S, typename T> point<S,T> operator* (const point<S,T>&, const vector<S,T>&);

    // iostream operators
    template <size_t S, typename T>
    std::ostream& operator<< (std::ostream&, const point<S,T>&);

    // Convenience typedefs
    typedef point<2,float> point2f;
    typedef point<3,float> point3f;

    typedef point<2,double> point2d;
    typedef point<3,double> point3d;
}

#include "point.ipp"

#endif // __UTIL_POINT_HPP
