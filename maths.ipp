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
