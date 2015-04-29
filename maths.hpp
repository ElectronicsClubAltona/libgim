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
 * Copyright 2010-2014 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __MATHS_HPP
#define __MATHS_HPP

#include "debug.hpp"
#include "types/traits.hpp"

#include <cmath>
#include <cstdint>
#include <limits>
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
constexpr T
pow [[gnu::pure]] (T x, unsigned y);


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
constexpr T
divup [[gnu::pure]] (const T a, const U b)
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
constexpr int sign (int);
constexpr float sign (float);
constexpr double sign (double);


//-----------------------------------------------------------------------------
// Comparisons
template <typename T>
bool
almost_equal [[gnu::pure]] (const T &a, const T &b)
    { return a == b; }


template <>
bool
almost_equal [[gnu::pure]] (const float &a, const float &b);


template <>
bool
almost_equal [[gnu::pure]] (const double &a, const double &b);


template <typename Ta, typename Tb>
typename std::enable_if<
    std::is_arithmetic<Ta>::value && std::is_arithmetic<Tb>::value,
    bool
>::type
almost_equal [[gnu::pure]] (Ta a, Tb b) {
    return almost_equal <decltype(a + b)> (static_cast<decltype(a + b)>(a),
                                           static_cast<decltype(a + b)>(b));
}


template <typename Ta, typename Tb>
typename std::enable_if<
    !std::is_arithmetic<Ta>::value || !std::is_arithmetic<Tb>::value,
    bool
>::type
almost_equal [[gnu::pure]] (const Ta &a, const Tb &b)
    { return a == b; }


// Useful for explictly ignore equality warnings
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
template <typename T, typename U>
bool
exactly_equal [[gnu::pure]] (const T &a, const U &b)
    { return a == b; }
#pragma GCC diagnostic pop


template <typename T>
bool
almost_zero [[gnu::pure]] (T a)
    { return almost_equal (a, 0); }


template <typename T>
bool
exactly_zero [[gnu::pure]] (T a)
    { return exactly_equal (a, static_cast<T> (0)); }


//-----------------------------------------------------------------------------
// angles, trig

template <typename T>
constexpr T PI = T(3.141592653589793238462643);

template <typename T>
constexpr T E = T(2.71828182845904523536028747135266250);


template <typename T>
constexpr T
to_degrees [[gnu::pure]] (T radians)
{
    return radians * 180 / PI<T>;
}


template <typename T>
constexpr T
to_radians [[gnu::pure]] (T degrees)
{
    return degrees / 180 * PI<T>;
}


//! Normalised sinc function
template <typename T>
constexpr T
sincn [[gnu::pure]] (T x)
{
    return almost_zero (x) ? 1 : std::sin (PI<T> * x) / (PI<T> * x);
}


//! Unnormalised sinc function
template <typename T>
constexpr T
sincu [[gnu::pure]] (T x)
{
    return almost_zero (x) ? 1 : std::sin (x) / x;
}


//-----------------------------------------------------------------------------
constexpr uintmax_t
factorial [[gnu::pure]] (unsigned i)
{
    return i <= 1 ? 0 : i * factorial (i - 1);
}


constexpr uintmax_t
stirling [[gnu::pure]] (unsigned n)
{
    return static_cast<uintmax_t> (std::sqrt (2 * PI<float> * n) * std::pow (n / E<float>, n));
}


constexpr uintmax_t
combination [[gnu::pure]] (unsigned n, unsigned k)
{
    return factorial (n) / (factorial (k) / (factorial (n - k)));
}


//-----------------------------------------------------------------------------
/// Variadic minimum
namespace util {
    template <typename T>
    constexpr T
    min [[gnu::pure]] (const T a)
        { return a; }


    template <typename T, typename U, typename ...Args>
    constexpr typename std::enable_if<
        std::is_unsigned<typename std::decay<T>::type>::value == std::is_unsigned<typename std::decay<U>::type>::value &&
        std::is_integral<typename std::decay<T>::type>::value == std::is_integral<typename std::decay<U>::type>::value,
        typename std::common_type<T,U>::type
    >::type
    min [[gnu::pure]] (const T a, const U b, Args ...args)
    {
        return min (a < b ? a : b, std::forward<Args> (args)...);
    }


//-----------------------------------------------------------------------------
    /// Variadic maximum
    template <typename T>
    constexpr T
    max [[gnu::pure]] (const T a)
        { return a; }


    template <typename T, typename U, typename ...Args>
    constexpr typename std::enable_if<
        std::is_unsigned<typename std::decay<T>::type>::value == std::is_unsigned<typename std::decay<U>::type>::value &&
        std::is_integral<typename std::decay<T>::type>::value == std::is_integral<typename std::decay<U>::type>::value,
        typename std::common_type<T,U>::type
    >::type
    max [[gnu::pure]] (const T a, const U b, Args ...args)
    {
        return max (a > b ? a : b, std::forward<Args> (args)...);
    }
}

//-----------------------------------------------------------------------------
// Limiting functions

// min/max clamping
template <typename T, typename U, typename V>
T
limit [[gnu::pure]] (const T val, const U lo, const V hi)
{
    CHECK_LE(
        decltype (lo+hi) (lo),
        decltype (hi+lo) (hi)
    );

    return val > hi ? hi:
           val < lo ? lo:
           val;
}


// clamped cubic hermite interpolation
template <typename T>
T
smoothstep [[gnu::pure]] (T a, T b, T x)
{
    CHECK_LE(a, b);
    x = limit ((x - a) / (b - a), T{0}, T{1});
    return x * x * (3 - 2 * x);
}

#include "types/string.hpp"

//-----------------------------------------------------------------------------
template <typename T, typename U>
U
renormalise [[gnu::pure]] (T t)
{
    static const T T_max = std::numeric_limits<T>::max ();
    static const U U_max = std::numeric_limits<U>::max ();
    static const bool shrinking = sizeof (U) < sizeof (T);
    static const bool T_float = std::is_floating_point<T>::value;
    static const bool U_float = std::is_floating_point<U>::value;

    if (T_float && U_float)
        return U (t);

    if (T_float) {
        return U (limit (t, 0, 1) * U_max);
    }

    if (U_float)
        return U(U (t) / T_max);

    if (shrinking)
        return U (t / (sizeof (T) / sizeof (U)));
    else
        return U (t) * (sizeof (U) / sizeof (T));
}

#include "maths.ipp"

#endif // __MATHS_HPP
