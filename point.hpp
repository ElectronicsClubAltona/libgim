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
    ///
    /// \tparam S number of dimensions
    /// \tparam T the underlying per-dimension datatype
    template <size_t S, typename T>
    struct point : public coord::base<S,T,point<S,T>>
    {
        using coord::base<S,T,point<S,T>>::base;

        vector<S,T> to (point) const;
        vector<S,T> from (point) const;

        template <size_t D> point<D,T> homog (void) const;

        static constexpr point<S,T> origin (void);

        void sanity (void) const;
    };

    ///////////////////////////////////////////////////////////////////////////
    // distance operators

    /// computes the exact euclidean distance between two points.
    template <size_t S, typename T, typename U>
    typename std::common_type<T,U>::type distance  (point<S,T>, point<S,U>);

    /// computes the squared euclidean distance between two points.
    ///
    /// useful if you just need to compare distances because it avoids a sqrt
    /// operation.
    template <size_t S, typename T, typename U>
    constexpr typename std::common_type<T,U>::type distance2 (point<S,T>, point<S,U>);

    /// computes the octile distance between two points. that is, the shortest
    /// distance between `a' and `b' where travel is only allowed beween the 8
    /// grid neighbours and cost for diagonals is proportionally larger than
    /// cardinal movement. see also: chebyshev.
    template <typename T, typename U>
    typename std::common_type<T,U>::type octile (point<2,T>, point<2,U>);

    /// computes the manhattan distance between two points. that is, the
    /// distance where travel is only allowed along cardinal directions.
    template <size_t S, typename T, typename U>
    constexpr typename std::common_type<T,U>::type manhattan (point<S,T>, point<S,U>);

    /// computes the cheyvshev distance between two points. that is, the
    /// shortest distance between `a' and `b' where travel is only allowed
    /// between the 8 grid neighbours and cost for diagonals is the same as
    /// cardinal movement. see also: octile.
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

#include "point.ipp"

#endif // __UTIL_POINT_HPP
