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
 * Copyright 2010-2014 Danny Robson <danny@nerdcruft.net>
 */

#include "maths.hpp"

#include "debug.hpp"

#include "float.hpp"

#include <cmath>
#include <type_traits>


//-----------------------------------------------------------------------------
template <typename T>
bool
is_pow2 (T value) {
    typedef typename std::enable_if<std::is_integral<T>::value, bool>::type return_type;
    return (return_type)(value && !(value & (value - 1)));
}


template bool is_pow2 (uint8_t);
template bool is_pow2 (uint16_t);
template bool is_pow2 (uint32_t);
template bool is_pow2 (uint64_t);


//-----------------------------------------------------------------------------
template <typename T>
T
log2up (T v)
{
    return log2 ((v << 1) - 1);
}

template uint32_t log2up (uint32_t);
template uint64_t log2up (uint64_t);


//-----------------------------------------------------------------------------
template <typename T>
T
log2 (T v) {
    static_assert (std::is_integral<T>::value,
                   "log2 is only implemented for integers");

    T l = 0;
    while (v >>= 1)
        ++l;

    return l;
}

template uint8_t  log2 (uint8_t);
template uint16_t log2 (uint16_t);
template uint32_t log2 (uint32_t);
template uint64_t log2 (uint64_t);


//-----------------------------------------------------------------------------
template <typename T>
double
rootsquare (T a, T b)
    { return sqrt (pow2 (a) + pow2 (b)); }


template double rootsquare (double, double);
template double rootsquare (   int,    int);


//-----------------------------------------------------------------------------
template <typename T>
bool
is_integer (const T &value) {
    T integer;
    return exactly_equal (std::modf (value, &integer),
                          static_cast<T> (0.0));
}


template bool is_integer (const double&);
template bool is_integer (const  float&);


//-----------------------------------------------------------------------------
template <>
unsigned
digits (const uint32_t &v) {
    return (v >= 1000000000) ? 10 :
           (v >=  100000000) ?  9 :
           (v >=   10000000) ?  8 :
           (v >=    1000000) ?  7 :
           (v >=     100000) ?  6 :
           (v >=      10000) ?  5 :
           (v >=       1000) ?  4 :
           (v >=        100) ?  3 :
           (v >=         10) ?  2 :
                                1;
}


//-----------------------------------------------------------------------------
template <>
bool
almost_equal (const float &a, const float &b)
    { return ieee_single::almost_equal (a, b); }


template <>
bool
almost_equal (const double &a, const double &b)
    { return ieee_double::almost_equal (a, b); }


//-----------------------------------------------------------------------------
template <typename T>
T
round_pow2 (T value) {
    typedef typename std::enable_if<std::is_integral<T>::value, T>::type return_type;

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


//-----------------------------------------------------------------------------
template <typename T>
int
sign (T val) {
    return val >= 0 ? 1 : -1;
}


template <>
int
sign (float val) {
    return static_cast<int> (copysign (1.0f, val));
}


template <>
int
sign (double val) {
    return static_cast<int> (copysign (1.0, val));
}


template int sign (int);


//-----------------------------------------------------------------------------
// Simple instantiations. Some functions aren't used internally to the library
// so it's easier to instantiate early and check for broken code at library
// build time.

template float limit (float, float, float);

template float smoothstep (float, float, float);
template double smoothstep (double, double, double);
