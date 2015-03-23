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

#include "debug.hpp"
#include "types/traits.hpp"

#include <cstdint>
#include <type_traits>
#include <utility>
#include <cmath>

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
struct constants { };

constexpr double PI_d = 3.141592653589793238462643;
constexpr float  PI_f = 3.141592653589793238462643f;

constexpr float  E_f = 2.71828182845904523536028747135266250f;
constexpr double E_d = 2.71828182845904523536028747135266250;

template <typename T>
constexpr T
to_degrees [[gnu::pure]] (T radians)
{
    return radians * 180 / constants<T>::PI;
}


template <typename T>
constexpr T
to_radians [[gnu::pure]] (T degrees)
{
    return degrees / 180 * constants<T>::PI;
}


//! Normalised sinc function
template <typename T>
constexpr T
sincn [[gnu::pure]] (T x)
{
    return almost_zero (x) ? 1 : std::sin (constants<T>::PI * x) / (constants<T>::PI * x);
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
    return static_cast<uintmax_t> (std::sqrt (2 * PI_f * n) * std::pow (n / E_f, n));
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


#include "maths.ipp"

#endif // __MATHS_HPP
