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
 * Copyright 2011-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_POINT_HPP
#define __UTIL_POINT_HPP

#include "extent.hpp"
#include "vector.hpp"
#include "coord.hpp"

#include <array>
#include <initializer_list>
#include <iostream>
#include <type_traits>

namespace util {
    /// An n-dimensional position in space.
    template <size_t S, typename T>
    struct point : public coord::base<S,T,coord::xyzw>
    {
        using coord::base<S,T,coord::xyzw>::base;

        // point operators
        template <typename U> typename std::common_type<T,U>::type distance  (const point<S,U> &) const;
        template <typename U> typename std::common_type<T,U>::type distance2 (const point<S,U> &) const;
        template <typename U> typename std::common_type<T,U>::type manhattan (const point<S,U> &) const;

        vector<S,T> to (const point&) const;
        vector<S,T> from (const point&) const;

        template <size_t D> point<D,T> redim (void) const;
        template <size_t D> point<D,T> redim (const util::point<D,T> &fill) const;
        template <size_t D> point<D,T> redim (T fill) const;

        template <size_t D> point<D,T> homog (void) const;

        static const point<S,T> ORIGIN;

        template<typename U> point<S,U> cast (void) const;

        void sanity (void) const;
    };

    // iostream operators
    template <size_t S, typename T>
    std::ostream& operator<< (std::ostream&, const point<S,T>&);

    // Convenience typedefs
    typedef point<2,float> point2f;
    typedef point<3,float> point3f;
    typedef point<4,float> point4f;

    typedef point<2,double> point2d;
    typedef point<3,double> point3d;

    typedef point<2,size_t> point2u;
    typedef point<2,intmax_t> point2i;
}

#include <functional>

namespace std {
    template <size_t S, typename T>
    struct hash<util::point<S,T>> {
        size_t operator() (const util::point<S,T> &p) const {
            std::hash<T> h;

            size_t k = 0;
            for (size_t i = 0; i < S; ++i)
                k = h (p.data[i] ^ k);

            return k;
        }
    };
}

#include "point.ipp"

#endif // __UTIL_POINT_HPP
