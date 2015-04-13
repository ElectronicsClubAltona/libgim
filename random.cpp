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


#include "random.hpp"

#include "range.hpp"

#include <type_traits>

using namespace util;

namespace util {
    template <typename T>
    T
    random (void) {
        static_assert (std::is_integral<T>::value, "random should only operate on integral types");
        return range<T>::UNLIMITED.random ();
    }

    template <>
    double
    random (void)
        { return range<double>::UNIT.random (); }

    template <>
    float
    random (void)
        { return range<float>::UNIT.random (); }

    template <>
    bool
    random (void)
        { return rand () & 0x01; }

    template <typename T>
    T&
    randomise (T &val)
        { return val = util::random<T> (); }
}

template double   util::random (void);
template float    util::random (void);
template uint64_t util::random (void);
template uint32_t util::random (void);
template uint16_t util::random (void);
