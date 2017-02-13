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

#include <type_traits>
#include <cstdint>


namespace util {
    const uint8_t BITMASK_1BITS = 0x01;
    const uint8_t BITMASK_2BITS = 0x03;
    const uint8_t BITMASK_3BITS = 0x07;
    const uint8_t BITMASK_4BITS = 0x0F;
    const uint8_t BITMASK_5BITS = 0x1F;
    const uint8_t BITMASK_6BITS = 0x3F;
    const uint8_t BITMASK_7BITS = 0x7F;
    const uint8_t BITMASK_8BITS = 0xFF;

    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    constexpr T
    rotatel [[gnu::pure]] (const T value, std::size_t magnitude)
    {
        return (value << magnitude) | (value >> sizeof (value) * 8 - magnitude);
    }


    template <typename T>
    constexpr T
    rotater [[gnu::pure]] (const T value, std::size_t magnitude)
    {
        return (value >> magnitude) | (value << sizeof (value) * 8 - magnitude);
    }


    ///////////////////////////////////////////////////////////////////////////
    // adapted from 'bit twiddling hacks'
    template <typename T>
    constexpr
    std::enable_if_t<std::is_integral<T>::value, T>
    reverse (T src)
    {
        T dst = src; // dst will be reversed bits of v; first get LSB of v

        int bits = sizeof (src) * 8 - 1; // extra shift needed at end
        for (src >>= 1; src; src >>= 1) {
            dst <<= 1;
            dst  |= src & 1;
            bits--;
        }

        dst <<= bits; // shift when src's highest bits are zero
        return dst;
    }


    ///////////////////////////////////////////////////////////////////////////
    // adapted from 'bit twiddling hacks'
    template <>
    constexpr
    uint8_t
    reverse (uint8_t val)
    {
        return ((val * 0x80200802ULL) & 0x0884422110ULL) * 0x0101010101ULL >> 32;
    }


    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    constexpr T
    popcount (std::enable_if_t<std::is_integral<T>::value,T> t)
    {
        return __builtin_popcount (t);
    }
}

#endif
