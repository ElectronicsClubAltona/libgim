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

#ifndef __UTIL_RAY_HPP
#define __UTIL_RAY_HPP

#include "point.hpp"
#include "vector.hpp"
#include "plane.hpp"
#include "aabb.hpp"

namespace util {
    template <size_t S, typename T>
    struct ray {
        ray (util::point<S,T> origin,
              util::vector<S,T> direction);

        T intersect (plane<S,T>) const;
        T intersect (AABB<S,T>) const;

        T closest (point<S,T>) const;

        util::point<S,T> at (T) const;

        util::point<S,T> p;
        util::vector<S,T> d;
    };


    typedef ray<2,float> ray2f;
    typedef ray<3,float> ray3f;
}

#endif
