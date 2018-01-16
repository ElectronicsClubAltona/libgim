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

#ifndef __UTIL_HASH_FASTHASH_HPP
#define __UTIL_HASH_FASTHASH_HPP

#include "../view.hpp"

#include <cstddef>
#include <cstdint>


// Zilong Tan's FastHash, via George Marsaglia's "Xorshift RNGs"
namespace util::hash {
    template <typename ValueT>
    struct fasthash {
        using digest_t = ValueT;

        static constexpr
        uint64_t mix (uint64_t v)
        {
            v ^= v >> 23;
            v *= 0x2127599bf4325c37;
            v ^= v >> 47;

            return v;
        }


        digest_t operator() (uint64_t seed, util::view<const uint8_t*>) const;
    };
}

#endif
