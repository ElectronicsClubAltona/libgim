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
 * Copyright 2010 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_RANDOM_HPP
#define __UTIL_RANDOM_HPP

#include <iterator>

namespace util {
    template <typename T>
    T& randomise (T &);

    template <typename T, size_t N>
    T* randomise (T(&)[N]);

    template <typename T>
    T random (void);

    template <typename T>
    typename T::value_type&
    choose (T &container) {
        typename T::iterator  cursor = container.begin ();
        typename T::size_type size   = container.size ();
        typename T::size_type offset = random<typename T::size_type> () % size;

        std::advance (cursor, offset);
        return *cursor;
    }

    template <typename T, size_t N>
    T&
    choose (T (&v)[N]) {
        return v[static_cast<size_t> (random<float> () * N)];
    }

    template <typename T>
    typename T::value_type&
    choose (T begin, T end) {
        typename T::difference_type size = std::distance (begin, end);
        std::advance (begin, random<T::size_type> () % size);
        return *begin;
    }
}

#include "random.ipp"

#endif
