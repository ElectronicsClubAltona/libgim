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

#ifndef __MATHS_HPP
#define __MATHS_HPP

#include <type_traits>
#include <utility>

template <typename T>
T
abs (T value)
{ return value > 0 ? value : -value; }


//-----------------------------------------------------------------------------
// Exponentials
template <typename T>
constexpr T
pow2 [[gnu::pure]] (T value)
    { return value * value; }


template <typename T>
T
pow (T x, unsigned y);


template <typename T>
bool
is_pow2 [[gnu::pure]] (T value);


//-----------------------------------------------------------------------------
// Logarithms
template <typename T>
T
log2 [[gnu::pure]] (T val);


template <typename T>
T
log2up [[gnu::pure]] (T val);


//-----------------------------------------------------------------------------
// Roots
template <typename T>
double
rootsquare [[gnu::pure]] (T a, T b);


//-----------------------------------------------------------------------------
// Rounding
template <typename T, typename U>
typename std::common_type<T, U>::type
align [[gnu::pure]] (T value, U size);


template <typename T>
T
round_pow2 [[gnu::pure]] (T value);


template <typename T, typename U>
T
divup (const T a, const U b)
    { return (a + b - 1) / b; }


//-----------------------------------------------------------------------------
// Classification
template <typename T>
bool
is_integer [[gnu::pure]] (const T& value);


//-----------------------------------------------------------------------------
// Properties
template <typename T>
unsigned
digits [[gnu::pure]] (const T& value);


//-----------------------------------------------------------------------------
template <typename T>
int sign (T val);


//-----------------------------------------------------------------------------
// Comparisons
template <typename T>
bool
almost_equal (const T &a, const T &b)
    { return a == b; }


template <>
bool
almost_equal (const float &a, const float &b);


template <>
bool
almost_equal (const double &a, const double &b);


template <typename Ta, typename Tb>
typename std::enable_if<
    std::is_arithmetic<Ta>::value && std::is_arithmetic<Tb>::value,
    bool
>::type
almost_equal (Ta a, Tb b) {
    return almost_equal <decltype(a + b)> (static_cast<decltype(a + b)>(a),
                                           static_cast<decltype(a + b)>(b));
}


template <typename Ta, typename Tb>
typename std::enable_if<
    !std::is_arithmetic<Ta>::value || !std::is_arithmetic<Tb>::value,
    bool
>::type
almost_equal (const Ta &a, const Tb &b)
    { return a == b; }


// Useful for explictly ignore equality warnings
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
template <typename T, typename U>
bool
exactly_equal (const T &a, const U &b)
    { return a == b; }
#pragma GCC diagnostic pop


template <typename T>
bool
almost_zero (T a)
    { return almost_equal (a, 0); }


template <typename T>
bool
exactly_zero (T a)
    { return exactly_equal (a, static_cast<T> (0)); }


//-----------------------------------------------------------------------------
// angles, trig
constexpr double PI_d = 3.141592653589793238462643;
constexpr float  PI_f = 3.141592653589793238462643f;

//-----------------------------------------------------------------------------
constexpr double
to_degrees (double radians) {
    return radians * 180 / PI_d;
}


constexpr float
to_degrees (float radians) {
    return radians * 180 / PI_f;
}


//-----------------------------------------------------------------------------
constexpr float
to_radians (float degrees) {
    return degrees / 180 * static_cast<float> (PI_f);
}


constexpr double
to_radians (double degrees) {
    return degrees / 180 * PI_d;
}


//-----------------------------------------------------------------------------
/// Variadic minimum
template <typename T>
constexpr T
min (const T a)
    { return a; }


template <typename T, typename U, typename ...Args>
constexpr typename std::enable_if<
    std::is_unsigned<typename std::decay<T>::type>::value == std::is_unsigned<typename std::decay<U>::type>::value &&
    std::is_integral<typename std::decay<T>::type>::value == std::is_integral<typename std::decay<U>::type>::value,
    typename std::common_type<T,U>::type
>::type
min (const T a, const U b, Args ...args)
{
    return min (a < b ? a : b, std::forward<Args> (args)...);
}


//-----------------------------------------------------------------------------
/// Variadic maximum
template <typename T>
constexpr T
max (const T a)
    { return a; }


template <typename T, typename U, typename ...Args>
constexpr typename std::enable_if<
    std::is_unsigned<typename std::decay<T>::type>::value == std::is_unsigned<typename std::decay<U>::type>::value &&
    std::is_integral<typename std::decay<T>::type>::value == std::is_integral<typename std::decay<U>::type>::value,
    typename std::common_type<T,U>::type
>::type
max (const T a, const U b, Args ...args)
{
    return max (a > b ? a : b, std::forward<Args> (args)...);
}


//-----------------------------------------------------------------------------
template <typename T, typename U, typename V>
T
limit (const T val, const U hi, const V lo)
{
    return val > hi ? hi:
           val < lo ? lo:
           val;
}


#include "maths.ipp"

#endif // __MATHS_HPP
