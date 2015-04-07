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
 * Copyright 2014 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_MATHS_IPP
#define __UTIL_MATHS_IPP
#else
#error "Include only once"
#endif

#include <type_traits>

//-----------------------------------------------------------------------------
template <typename T, typename U>
typename std::common_type<T,U>::type
align (T value, U size) {
    static_assert (std::is_integral<T>::value, "align requires integral types");
    static_assert (std::is_integral<U>::value, "align requires integral types");

    return divup (value, size) * size;
}


//-----------------------------------------------------------------------------
template <typename T>
constexpr T
pow (T x, unsigned y)
{
    return y == 0 ? 1 : x * pow (x, y - 1);
}


///----------------------------------------------------------------------------
/// Return a unit type with a sign that matches the provided value
///
/// We were using __builtin_signbit for the potential speedboost, but it causes
/// problems with constexpr under clang. If you need speed then you'll probably
/// have to handcode something.
constexpr int
sign (int v)
{
    return std::signbit (v) ? -1 : 1;
}

constexpr float
sign (float v)
{
    return std::signbit (v) ? -1.f : 1.f;
}


constexpr double
sign (double v)
{
    return std::signbit (v) ? -1. : 1.f;
}


//-----------------------------------------------------------------------------
template <>
struct constants<float>
{
    static constexpr float PI = PI_f;
    static constexpr float E  = E_f;
};


template <>
struct constants<double>
{
    static constexpr double PI = PI_d;
    static constexpr double E  = E_d;
};
