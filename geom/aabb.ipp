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

#if defined(__UTIL_GEOM_AABB_IPP)
#error
#endif

#define __UTIL_GEOM_AABB_IPP

#include "sample.hpp"

#include <random>


///////////////////////////////////////////////////////////////////////////////
namespace util::geom {
    template <size_t S, typename T, typename G>
    struct sampler<S,T,aabb,G> {
        static point<S,T>
        fn (aabb<S,T> b, G &g)
        {
            std::uniform_real_distribution<T> d;

            point<S,T> p;
            std::generate (p.begin (), p.end (), [&] (void) { return d (g); });

            return p * (b.p1 - b.p0) + b.p0.template as<util::vector> ();
        }
    };
}
