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


namespace util {
    template <typename T> T abs (T t) { return ::abs<T> (t); }
}


///////////////////////////////////////////////////////////////////////////////
// exponentials

namespace util {
    template <typename T>
    constexpr T
    pow2 [[gnu::pure]] (T value)
        { return value * value; }
}

template <typename T> constexpr T pow2 [[gnu::pure]] (T value) { return util::pow2 (value); }

//-----------------------------------------------------------------------------
template <typename T>
constexpr T
pow [[gnu::pure]] (T x, unsigned y);

namespace util {

    template <typename T>
    constexpr T pow (T x, unsigned y) { return ::pow (x, y); }
}


//-----------------------------------------------------------------------------
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
// factorisation
template <typename T>
constexpr T
gcd (T a, T b)
{
    if (a == b) return a;

    if (a > b) return gcd (a - b, b);
    if (b > a) return gcd (a, b - a);

    unreachable ();
}


//-----------------------------------------------------------------------------
constexpr int sign (int);
constexpr float sign (float);
constexpr double sign (double);


//-----------------------------------------------------------------------------
template <typename T>
const T&
identity (const T& t)
{
    return t;
}


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
    static_assert (std::is_floating_point<T>::value, "undefined for integral types");
    return radians * 180 / PI<T>;
}


template <typename T>
constexpr T
to_radians [[gnu::pure]] (T degrees)
{
    static_assert (std::is_floating_point<T>::value, "undefined for integral types");
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


/// stirlings approximation of factorials
constexpr uintmax_t
stirling [[gnu::pure]] (unsigned n)
{
    return static_cast<uintmax_t> (
        std::sqrt (2 * PI<float> * n) * std::pow (n / E<float>, n)
    );
}


constexpr uintmax_t
combination [[gnu::pure]] (unsigned n, unsigned k)
{
    return factorial (n) / (factorial (k) / (factorial (n - k)));
}


//-----------------------------------------------------------------------------
// kahan summation for long floating point sequences

template <class InputIt>
typename std::iterator_traits<InputIt>::value_type
fsum (InputIt first, InputIt last)
{
    using T = typename std::iterator_traits<InputIt>::value_type;
    static_assert (std::is_floating_point<T>::value,
                   "fsum only works for floating point types");

    T sum = 0;
    T c = 0;

    for (auto cursor = first; cursor != last; ++cursor) {
        T y = *cursor - c;
        T t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }

    return sum;
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
constexpr T
limit [[gnu::pure]] (const T val, const U lo, const V hi)
{
    lo <= hi ? (void)0 : panic ();

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


///////////////////////////////////////////////////////////////////////////////
// renormalisation of unit floating point and/or normalised integers

// int -> float
template <typename T, typename U>
constexpr
typename std::enable_if<
    !std::is_floating_point<T>::value && std::is_floating_point<U>::value, U
>::type
renormalise [[gnu::pure]] (T t)
{
    return t / static_cast<U> (std::numeric_limits<T>::max ());
}


// float -> int
template <typename T, typename U>
constexpr
typename std::enable_if<
    std::is_floating_point<T>::value && !std::is_floating_point<U>::value, U
>::type
renormalise [[gnu::pure]] (T t)
{
    // Ideally std::ldexp would be involved but it complicates handing
    // integers with greater precision than our floating point type. Also it
    // would prohibit constexpr and involve errno.

    size_t usable    = std::numeric_limits<T>::digits;
    size_t available = sizeof (U) * 8;
    size_t shift     = std::max (available, usable) - usable;

    t = limit (t, 0, 1);

    // construct an integer of the float's mantissa size, multiply it by our
    // parameter, then shift it back into the full range of the integer type.
    U in  = std::numeric_limits<U>::max () >> shift;
    U mid = static_cast<U> (t * in);
    U out = mid << shift;

    // use the top bits of the output to fill the bottom bits which through
    // shifting would otherwise be zero. this gives us the full extent of the
    // integer range, while varying predictably through the entire output
    // space.
    return out | out >> (available - shift);
}


// float -> float, avoid identity conversion as we don't want to create
// ambiguous overloads
template <typename T, typename U>
constexpr
typename std::enable_if<
    std::is_floating_point<T>::value &&
    std::is_floating_point<U>::value &&
    !std::is_same<T,U>::value, U
>::type
renormalise [[gnu::pure]] (T t)
{
    return static_cast<U> (t);
}


// hi_int -> lo_int
template <typename T, typename U>
constexpr
typename std::enable_if<
    std::is_integral<T>::value &&
    std::is_integral<U>::value &&
    (sizeof (T) > sizeof (U)), U
>::type
renormalise [[gnu::pure]] (T t)
{
    static_assert (sizeof (T) > sizeof (U),
                   "assumes right shift is sufficient");

    // we have excess bits ,just shift and return
    constexpr auto shift = 8 * (sizeof (T) - sizeof (U));
    return t >> shift;
}


// lo_int -> hi_int
template <typename T, typename U>
constexpr
typename std::enable_if<
    std::is_integral<T>::value &&
    std::is_integral<U>::value &&
    sizeof (T) < sizeof (U), U
>::type
renormalise [[gnu::pure]] (T t)
{
    static_assert (sizeof (T) < sizeof (U),
                   "assumes bit creation is required to fill space");

    // we need to create bits. fill the output integer with copies of ourself.
    // this is approximately correct in the general case (introducing a small
    // linear positive bias), but allows us to fill the output space in the
    // case of input maximum.

    static_assert (sizeof (U) % sizeof (T) == 0,
                   "assumes integer multiple of sizes");

    U out = 0;

    for (size_t i = 0; i < sizeof (U) / sizeof (T); ++i)
        out |= U (t) << sizeof (T) * 8 * i;

    return out;
}


template <typename T, typename U>
constexpr
typename std::enable_if<
    std::is_same<T,U>::value, U
>::type
renormalise [[gnu::pure]] (T t)
{ return t; }

#include "maths.ipp"

#endif // __MATHS_HPP
