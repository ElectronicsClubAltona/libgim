/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
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

#endif 

