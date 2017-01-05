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

#if defined(__UTIL_GEOM_SAMPLE_IPP)
#error
#endif

#define __UTIL_GEOM_SAMPLE_IPP

#include "ops.hpp"

namespace util::geom {
    // use rejection sampling by default
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K,
        typename G
    >
    point<S,T>
    sampler<S,T,K,G>::fn (K<S,T> k, G &g)
    {
        auto b = bounds (k);

        while (true) {
            auto p = sample (b, g);
            if (intersects (k, p))
                return p;
        }
    }
}

