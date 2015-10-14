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

#if defined(__UTIL_GEOM_ELLIPSE_IPP)
#error
#endif

#define __UTIL_GEOM_ELLIPSE_IPP

#include "sample.hpp"

#include <cmath>
#include <random>

///////////////////////////////////////////////////////////////////////////////
namespace util { namespace geom {
    template <typename T, template <size_t,typename> class K, typename G>
    struct sampler<2,T,K,G>
    {
        static util::point<2,T> fn (K<2,T> k, G &g)
        {
            std::uniform_real_distribution<T> dist;

            float phi = dist (g) * 2 * PI<T>;
            float rho = std::sqrt (dist (g));

            return util::point<2,T> {
                std::cos (phi),
                std::sin (phi)
            } * rho * k.radius + k.origin.template as<util::vector> ();
        }
    };
} }
