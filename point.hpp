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
 * Copyright 2011-2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_POINT_HPP
#define __UTIL_POINT_HPP

#include "vector.hpp"
#include "coord.hpp"

#include <type_traits>

namespace util {
    /// An n-dimensional position in space.
    template <size_t S, typename T>
    struct point : public coord::base<S,T,point,coord::xyzw>
    {
        using coord::base<S,T,util::point,coord::xyzw>::base;

        vector<S,T> to (point) const;
        vector<S,T> from (point) const;

        template <size_t D> point<D,T> homog (void) const;

        static const point<S,T> ORIGIN;

        void sanity (void) const;
    };

    // distance operators
    template <size_t S, typename T, typename U>
    typename std::common_type<T,U>::type distance  (point<S,T>, point<S,U>);

    template <size_t S, typename T, typename U>
    constexpr typename std::common_type<T,U>::type distance2 (point<S,T>, point<S,U>);

    template <typename T, typename U>
    typename std::common_type<T,U>::type octile (point<2,T>, point<2,U>);

    template <size_t S, typename T, typename U>
    constexpr typename std::common_type<T,U>::type manhattan (point<S,T>, point<S,U>);

    template <size_t S, typename T, typename U>
    constexpr typename std::common_type<T,U>::type chebyshev (point<S,T>, point<S,U>);

    // Convenience typedefs
    template <typename T> using point1 = point<1,T>;
    template <typename T> using point2 = point<2,T>;
    template <typename T> using point3 = point<3,T>;
    template <typename T> using point4 = point<4,T>;

    template <size_t S> using pointi = point<S,int>;
    template <size_t S> using pointf = point<S,float>;

    typedef point1<float> point1f;
    typedef point2<float> point2f;
    typedef point3<float> point3f;
    typedef point4<float> point4f;

    typedef point2<double> point2d;
    typedef point3<double> point3d;
    typedef point4<double> point4d;

    typedef point1<unsigned> point1u;
    typedef point2<unsigned> point2u;
    typedef point3<unsigned> point3u;
    typedef point4<unsigned> point4u;

    typedef point2<int> point2i;
    typedef point3<int> point3i;
    typedef point4<int> point4i;
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
