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

#include "./rand/permute.hpp"
#include "./rand/hash.hpp"

#include <cstdint>

namespace util { namespace noise {
    using seed_t = uint64_t;
} }

namespace util { namespace noise { namespace rand {
    /// generate a uniform random floating point in the range [0, 1] from a seed and vector
    template <
        typename U,
        size_t S,
        typename T,
        template <size_t,typename> class Q
    >
    U
    scalar (uint64_t seed, Q<S,T> value) noexcept
    {
        #if 1
        return permute::scalar<U> (seed, value);
        #else
        return hash::scalar<U> (seed, value);
        #endif
    }

    /// generate a coordinate type with uniform random components in the range [0, 1]
    template <
        template <size_t,typename> class R,
        typename U,
        size_t S,
        typename T,
        template <size_t,typename> class Q
    >
    R<S,U>
    coord (uint64_t seed, Q<S,T> value) noexcept
    {
        #if 1
        return permute::coord<R,U,S,T,Q> (seed, value);
        #else
        return hash::coord<R,U,S,T,Q> (seed, value);
        #endif
    }
} } }

#endif
