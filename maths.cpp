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
 * Copyright 2010 Danny Robson <danny@nerdcruft.net>
 */

#include "maths.hpp"

#include "enable_if.hpp"
#include "float.hpp"

#include <cmath>
#include <type_traits>


template <typename T>
T 
pow2 (T value)
    { return value * value; }

template double pow2(double);
template    int pow2(   int);


template <typename T>
bool
is_pow2 (T value) {
    typedef typename enable_if<std::is_integral<T>::value, bool>::type return_type;
    return (return_type)(value && !(value & (value - 1)));
}

template bool is_pow2 (uint8_t);
template bool is_pow2 (uint16_t);
template bool is_pow2 (uint32_t);
template bool is_pow2 (uint64_t);


template <typename T>
double
rootsquare (T a, T b)
    { return sqrt (pow2 (a) + pow2 (b)); }

template double rootsquare (double, double);
template double rootsquare (   int,    int);


template <typename T>
bool
is_integer (const T &value) {
    T integer;
    return exactly_equal (std::modf (value, &integer),
                          static_cast<T> (0.0));
}

template bool is_integer (const double&);
template bool is_integer (const  float&);


template <>
bool
almost_equal (const float &a, const float &b)
    { return ieee_single::almost_equal (a, b); }


template <>
bool
almost_equal (const double &a, const double &b)
    { return ieee_double::almost_equal (a, b); }


template <typename T>
typename enable_if<std::is_integral<T>::value, T>::type
round_up (T value, T align) {
    check_hard (align > 1);
    return (value + align - 1) / align;
}


template <typename T>
T
round_pow2 (T value) {
    typedef typename enable_if<std::is_integral<T>::value, T>::type return_type;

    --value;

    for (unsigned i = 1; i < sizeof (T) * 8; i <<= 1) {
        value |= value >> i;
    }

    ++value;
    return (return_type)value;
}


template uint8_t  round_pow2 (uint8_t);
template uint16_t round_pow2 (uint16_t);
template uint32_t round_pow2 (uint32_t);
template uint64_t round_pow2 (uint64_t);
