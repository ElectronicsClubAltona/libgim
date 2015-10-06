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


#ifndef __UTIL_NOISE_RAND_PERMUTE_HPP
#define __UTIL_NOISE_RAND_PERMUTE_HPP

#include <array>

///////////////////////////////////////////////////////////////////////////////
namespace util { namespace noise { namespace rand {
    struct permute {
        /// generate a uniform random floating point in the range [0, 1] from a seed and vector
        template <
            typename U,
            size_t S,
            typename T,
            template <size_t,typename> class Q
        >
        static U
        scalar (uint64_t seed, Q<S,T> value);

        /// generate a coordinate type with uniform random components in the range [0, 1]
        template <
            template <size_t,typename> class R,
            typename U,
            size_t S,
            typename T,
            template <size_t,typename> class Q
        >
        static R<S,U>
        coord (uint64_t seed, Q<S,T> value);

    private:
        static const std::array<uint8_t, 256> PERMUTE;
    };
} } }

#include "./permute.ipp"

#endif

