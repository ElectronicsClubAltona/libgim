/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
    struct point : public coord::base<S,T,point,coord::xyzw>
    {
        using coord::base<S,T,util::point,coord::xyzw>::base;

        // point operators
        template <typename U> typename std::common_type<T,U>::type distance  (point<S,U>) const;
        template <typename U> typename std::common_type<T,U>::type distance2 (point<S,U>) const;
        template <typename U> typename std::common_type<T,U>::type manhattan (point<S,U>) const;

        vector<S,T> to (point) const;
        vector<S,T> from (point) const;

        template <size_t D> point<D,T> homog (void) const;

        static const point<S,T> ORIGIN;

        void sanity (void) const;
    };

    // iostream operators
    template <size_t S, typename T>
    std::ostream& operator<< (std::ostream&, point<S,T>);

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
        size_t operator() (util::point<S,T> p) const {
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
