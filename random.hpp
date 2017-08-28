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
 * Copyright 2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef CRUFT_UTIL_RANDOM_HPP
#define CRUFT_UTIL_RANDOM_HPP

#include <random>


namespace util::random {
    ///////////////////////////////////////////////////////////////////////////
    /// return correctly initialised thread-local generator of an unspecified,
    /// but not entirely useless, type. ie, not LCG.
    auto&
    generator (void)
    {
        static thread_local std::mt19937_64 gen { std::random_device {}() };
        return gen;
    }


    ///////////////////////////////////////////////////////////////////////////
    /// select a value uniformly from the range [lo, hi)
    template <typename T>
    std::enable_if_t<std::is_floating_point_v<T>,T>
    uniform (T lo, T hi)
    {
        return std::uniform_real_distribution { lo, hi } (generator ());
    }


    //-------------------------------------------------------------------------
    /// select a value uniformly from the range [lo, hi)
    template <typename T>
    std::enable_if_t<std::is_integral_v<T>,T>
    uniform (T lo, T hi)
    {
        return std::uniform_int_distribution { lo, hi } (generator ());
    }


    ///////////////////////////////////////////////////////////////////////////
    /// choose a value at random from an array
    template <typename T, size_t N>
    T&
    choose (T (&t)[N])
    {
        std::uniform_int_distribution<size_t> dist (0, N - 1);
        return t[dist (generator ())];
    }
}

#endif
