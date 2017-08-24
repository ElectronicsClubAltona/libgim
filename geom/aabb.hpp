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
 * Copyright 2015-2017 Danny Robson <danny@nerdcruft.net>
 */


#ifndef CRUFT_UTIL_GEOM_AABB_HPP
#define CRUFT_UTIL_GEOM_AABB_HPP

#include "../point.hpp"
#include "../extent.hpp"

#include <cstdint>

namespace util::geom {
    ///////////////////////////////////////////////////////////////////////////
    template <size_t S, typename T>
    struct aabb {
        aabb () = default;
        aabb (point<S,T>, point<S,T>);

        T diameter (void) const;
        extent<S,T> magnitude (void) const;

        /// tests whether a point lies within the region, inclusive of borders
        bool inclusive (point<S,T>) const;

        point<S,T> closest (point<S,T>) const;

        aabb<S,T> expand (util::vector<S,T>) const noexcept;
        aabb<S,T> expand (T) const noexcept;

        aabb<S,T> contract (util::vector<S,T>) const noexcept;
        aabb<S,T> contract (T) const noexcept;

        void cover (point<S,T>);

        aabb<S,T> operator+ (vector<S,T>) const;
        aabb<S,T> operator- (vector<S,T>) const;

        point<S,T> p0;
        point<S,T> p1;
    };


    ///////////////////////////////////////////////////////////////////////////
    template <std::size_t S, typename T>
    constexpr bool
    operator== (const aabb<S,T> &a, const aabb<S,T> &b) noexcept
    {
        return a.p0 == b.p0 && a.p1 == b.p1;
    }


    ///////////////////////////////////////////////////////////////////////////
    typedef aabb<2,float> aabb2f;
    typedef aabb<2,unsigned> aabb2u;
    typedef aabb<2,int> aabb2i;

    typedef aabb<3,float> aabb3f;
    typedef aabb<3,unsigned> aabb3u;
    typedef aabb<3,int> aabb3i;
}

#include "aabb.ipp"

#endif
