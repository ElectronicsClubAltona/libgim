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


#ifdef __UTIL_NOISE_RAND_HASH_IPP
#error
#endif

#define __UTIL_NOISE_RAND_HASH_IPP

namespace util { namespace noise { namespace rand {
    //-------------------------------------------------------------------------
    template <
        typename U,
        size_t S,
        typename T,
        template <size_t,typename> class Q
    >
    U
    hash::scalar (uint64_t seed, Q<S,T> query)
    {
        constexpr decltype(seed) BITS = 0xFFFF;
        static_assert (std::is_integral<T>::value,
                       "mixing only works on integral types");

        uint64_t accum = seed;
        for (auto i: query)
            accum = util::hash::murmur2::mix (accum, i);

        return (accum & BITS) / U{BITS};
    }


    //-------------------------------------------------------------------------
    template <
        template <size_t,typename> class R,
        typename U,
        size_t S,
        typename T,
        template <size_t,typename> class Q
    >
    R<S,U>
    hash::coord (uint64_t seed, Q<S,T> query)
    {
        constexpr decltype(seed) PERTURB = 0x96c39996c36c36c3;
        constexpr decltype(seed) BITS    = 0xFFFF;

        auto accum = seed ^ PERTURB;
        for (auto i: query)
            accum = util::hash::murmur2::mix (accum, i);

        R<S,U> result;
        for (auto &i: result) {
            i  = (accum & BITS);
            i /= BITS;
            accum = util::hash::murmur2::mix (accum, seed);
        }

        return result;
    }
} } }
