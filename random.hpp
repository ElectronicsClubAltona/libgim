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

#ifndef __UTIL_RANDOM_HPP
#define __UTIL_RANDOM_HPP

#include <algorithm>
#include <random>


///////////////////////////////////////////////////////////////////////////////
namespace util::rand {
    //-------------------------------------------------------------------------
    using default_generator = std::minstd_rand;


    //-------------------------------------------------------------------------
    template <typename Generator = default_generator>
    Generator&
    thread_engine (void)
    {
        std::random_device rd;
        thread_local Generator gen (rd ());
        return gen;
    }


    //-------------------------------------------------------------------------
    template <typename Generator = default_generator, typename T, size_t N>
    T&
    choose (T (&t)[N], Generator gen = thread_engine<Generator> ())
    {
        std::uniform_int_distribution<size_t> dist (0, N-1);
        return t[dist (gen)];
    }


    //-------------------------------------------------------------------------
    template <
        typename T,
        typename IteratorT,
        typename GeneratorT = default_generator
    >
    void
    uniform (T lo, T hi, IteratorT first, IteratorT last)
    {
        static_assert (std::is_same_v<T, typename std::iterator_traits<IteratorT>::value_type>);
        auto &gen = thread_engine<GeneratorT> ();

        static_assert (std::is_integral_v<T> || std::is_floating_point_v<T>);

        if constexpr (std::is_integral_v<T>) {
            std::generate (
                first,
                last,
                [&gen, d = std::uniform_int_distribution<T> (lo, hi)]
            {
                return d (gen);
            });
        }

        if constexpr (std::is_floating_point_v<T>) {
            auto d = std::uniform_real_distribution<T> (lo, hi);
            std::generate (
                first,
                last,
                [&]
            {
                    return d (gen);
            });
        }
    }
};

#endif
