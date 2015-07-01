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


#ifdef __UTIL_NOISE_RAND_IPP
#error
#endif
#define __UTIL_NOISE_RAND_IPP

#include "../hash/murmur/murmur2.hpp"


namespace util { namespace noise {
    template <typename T, typename V>
    T
    rand (uint64_t seed, V value)
    {
        uint64_t accum = seed;
        for (auto i: value)
            accum = hash::murmur2::mix (accum, i);

        T out = (accum & 0xFFFF) / T{0xFFFF};

        out *= 2;
        out -= 1;

        return out;
    }


    template <size_t N, typename T, typename V>
    vector<N,T>
    rand (uint64_t seed, V value)
    {
        uint64_t accum = seed;
        for (auto i: value)
            accum = hash::murmur2::mix (accum, i);

        vector<N,T> out;
        for (auto &i: out) {
            i = (accum & 0xFFFF) / T{0xFFFF};
            accum = hash::murmur2::mix (accum, seed);
        }

        return out * 2 - 1;
    }
} }
