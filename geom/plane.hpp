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

#ifndef __UTIL_PLANE_HPP
#define __UTIL_PLANE_HPP

#include "../point.hpp"
#include "../vector.hpp"

namespace util::geom {
    template <size_t S, typename T>
    struct plane {
        plane (util::point<S,T> p,
               util::vector<S,T> n);

        util::point<S,T> p;
        util::vector<S,T> n;
    };


    typedef plane<2,float> plane2f;
    typedef plane<3,float> plane3f;
}

#endif
