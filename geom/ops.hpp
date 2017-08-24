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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_GEOM_OPS_HPP
#define __UTIL_GEOM_OPS_HPP

#include "aabb.hpp"
#include "../point.hpp"


///////////////////////////////////////////////////////////////////////////////
namespace util::geom {
    template <
        size_t S,
        typename T,
        template <size_t,typename> class A,
        template <size_t,typename> class B
    >
    bool
    intersects (A<S,T>, B<S,T>);

    template <
        size_t S,
        typename T,
        template <size_t,typename> class A,
        template <size_t,typename> class B
    >
    T
    distance2 (A<S,T>, B<S,T>);

    template <
        size_t S,
        typename T,
        template <size_t,typename> class A,
        template <size_t,typename> class B
    >
    T
    distance (A<S,T>, B<S,T>);

    template <
        size_t S,
        typename T,
        template <size_t,typename> class K
    >
    aabb<S,T>
    bounds (K<S,T>);

    template <
        size_t S,
        typename T,
        template <size_t,typename> class K
    >
    T
    diameter (K<S,T>);

    template <
        size_t S,
        typename T,
        template <size_t,typename> class K
    >
    point<S,T>
    closest (K<S,T>, point<S,T>);

    template <
        size_t S,
        typename T,
        template <size_t,typename> class K
    >
    vector<S,T>
    magnitude (K<S,T>);

    template <
        size_t S,
        typename T,
        template <size_t,typename> class K
    >
    K<S,T>
    scale (K<S,T>, T);
}

#endif
