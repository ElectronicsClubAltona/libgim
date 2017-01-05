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

#ifndef __UTIL_RAND_LCG_HPP
#define __UTIL_RAND_LCG_HPP

#include <cstdint>
#include <type_traits>


namespace util::rand {
    /// linear congruential generator
    ///
    /// T: output/state type
    /// M: modulus
    /// A: multiplier
    /// C: increment
    template <typename T, T M, T A, T C>
    struct lcg {
    public:
        using result_type = T;

        static_assert (std::is_unsigned<T>::value,
                       "LCG generates integer overflow which is undefined behaviour for signed types");
        lcg (T seed);

        result_type operator() (void);

        static result_type min (void);
        static result_type max (void);

        void discard (unsigned);

    private:
        T m_x;
    };

    // glibc: typedef lcg<uint32_t, pow2(31), 1103515245, 12345> lcg_t;
    using lcg_t = lcg<uint64_t,0u,6364136223846793005ul, 1ul>;
}

#endif
