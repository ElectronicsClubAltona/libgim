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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_BITWISE_HPP
#define __UTIL_BITWISE_HPP

#include <cstdint>

#include "debug.hpp"

const uint8_t BITMASK_1BITS = 0x01;
const uint8_t BITMASK_2BITS = 0x03;
const uint8_t BITMASK_3BITS = 0x07;
const uint8_t BITMASK_4BITS = 0x0F;
const uint8_t BITMASK_5BITS = 0x1F;
const uint8_t BITMASK_6BITS = 0x3F;
const uint8_t BITMASK_7BITS = 0x7F;
const uint8_t BITMASK_8BITS = 0xFF;

///////////////////////////////////////////////////////////////////////////////
#define MODT(x) ((x) % (sizeof (T) * 8))

template <typename T>
constexpr T
rotatel (const T &value, size_t magnitude) {
    return (value << MODT (magnitude)) |
           (value >> sizeof (value) * 8 - MODT (magnitude));
}


template <typename T>
constexpr T
rotater (const T &value, size_t magnitude) {
    return (value >> MODT (magnitude)) |
           (value << sizeof (value) * 8 - MODT (magnitude));
}

#undef MODT


///////////////////////////////////////////////////////////////////////////////
// TODO: make constexpr for C++14
template <typename T>
T
reverse (T value) {
    T out = value;

    size_t bits = sizeof (value) * 8 - 1;
    for (value >>= 1; value; value >>= 1) {
        out <<= 1;
        out  |= value & 0x01;
        --bits;
    }

    out <<= bits;
    return out;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
constexpr T
popcount (std::enable_if_t<std::is_integral<T>::value,T> t)
{
    return __builtin_popcount (t);
}

#endif

