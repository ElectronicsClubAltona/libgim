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

#ifndef __MATHS_HPP
#define __MATHS_HPP

#include "annotations.hpp"

#include <type_traits>
#include <utility>

template <typename T>
constexpr T
pow2 (T value)
    { return value * value; }


template <typename T>
bool
is_pow2 (T value) pure;


template <typename T>
double
rootsquare (T a, T b) pure;


template <typename T, typename U>
typename std::common_type<T, U>::type
round_up (T value, U align) pure;


template <typename T>
T
round_pow2 (T value) pure;


template <typename T>
bool
is_integer (const T& value) pure;


template <typename T>
unsigned
digits (const T& value) pure;


template <typename T, typename U>
T
divup (const T a, const U b)
    { return (a + b - 1) / b; }


/**
 * Check if two floating point numbers are approximately equal. Returns true
 * if the difference is less than a percentage of each individual value.
 *
 * @e maximum percentage difference for equal values
 */

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


const double PI = 3.141592653589793238462643;

inline double
to_degrees (double radians) {
    return radians * 180 / PI;
}

inline double
to_radians (double degrees) {
    return degrees / 180 * PI;
}


/// Variadic minimum
template <typename T>
const T&
min (const T &a)
    { return a; }


template <typename T, typename U, typename ...Args>
typename std::common_type<T, U>::type
min (const T a , const U b , const Args &...args )
    { return min ( b < a ? b : a, args...); }


/// Variadic maximum
template <typename T>
const T&
max (const T &a)
    { return a; }


template <typename T, typename ...Args>
const T&
max (const T &a , const T &b , const Args &...args )
    { return max ( b > a ? b : a, args...); }


template <typename T>
int sign (T val);


template <typename T, typename U, typename V>
T
limit (const T &&val, const U &&hi, const V &&lo) {
    return val > hi ? hi:
           val < lo ? lo:
           std::move (val);
}

#include "maths.ipp"

#endif // __MATHS_HPP
