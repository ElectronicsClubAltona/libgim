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

#ifndef __UTIL_NOISE_RAND_HPP
#define __UTIL_NOISE_RAND_HPP

namespace util { namespace noise {
    /// generate a uniform random floating point in the range [-1, 1] from a seed and vector
    template <
        typename U,
        size_t S,
        typename T,
        template <size_t,typename> class Q
    >
    U
    rand (uint64_t seed, Q<S,T> value);

    template <
        template <size_t,typename> class R,
        typename U,
        size_t S,
        typename T,
        template <size_t,typename> class Q
    >
    R<S,U>
    rand (uint64_t seed, Q<S,T> value);
} }

#include "rand.ipp"

#endif
