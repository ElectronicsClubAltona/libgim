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
 * Copyright 2011-2015 Danny Robson <danny@nerdcruft.net>
 */

#include "lerp.hpp"

#include "debug.hpp"
#include "maths.hpp"

#include <cmath>

using util::lerp::linear;
using util::lerp::cosine;
using util::lerp::cubic;
using util::lerp::quintic;
using util::lerp::truncate;


//-----------------------------------------------------------------------------
template <typename T>
T
truncate<T>::operator() (T a, T, T weight) noexcept
{
    static_assert (std::is_floating_point<T>::value,
                   "lerp is only defined for floating types");

    CHECK_LIMIT (weight, 0, 1);
    (void)weight;

    return a;
}

template struct util::lerp::truncate<float>;
template struct util::lerp::truncate<double>;


//-----------------------------------------------------------------------------
template <typename T>
T
linear<T>::operator() (T a, T b, T weight) noexcept
{
    static_assert (std::is_floating_point<T>::value,
                   "lerp is only defined for floating types");
    CHECK_LIMIT (weight, 0, 1);

    return a * (1 - weight) + b * weight;
}

template struct util::lerp::linear<float>;
template struct util::lerp::linear<double>;


//-----------------------------------------------------------------------------
template <typename T>
T
cosine<T>::operator() (T a, T b, T weight) noexcept
{
    static_assert (std::is_floating_point<T>::value,
                   "lerp is only defined for floating types");
    CHECK_LIMIT (weight, 0, 1);

    T t = (1 - std::cos (weight * PI<T>)) * T(0.5);
    return a * (1 - t) + b * t;
}

template struct util::lerp::cosine<float>;
template struct util::lerp::cosine<double>;


//-----------------------------------------------------------------------------
template <typename T>
T
cubic<T>::operator() (T a, T b, T weight) noexcept
{
    static_assert (std::is_floating_point<T>::value,
                   "lerp is only defined for floating types");
    CHECK_LIMIT (weight, 0, 1);

    // -2w^3 * 3w^2
    T t = weight * weight * (3 - 2 * weight);
    return a * (1 - t) + b * t;
}

template struct util::lerp::cubic<float>;
template struct util::lerp::cubic<double>;


//-----------------------------------------------------------------------------
template <typename T>
T
quintic<T>::operator() (T a, T b, T weight) noexcept
{
    static_assert (std::is_floating_point<T>::value,
                   "lerp is only defined for floating types");
    CHECK_LIMIT (weight, 0, 1);

    // from perlin's improved noise: 6w^5 -15w^4 +10w^3
    T t = weight * weight * weight * (weight * (weight * 6 - 15) + 10);
    return a * (1 - t) + b * t;
}

template struct util::lerp::quintic<float>;
template struct util::lerp::quintic<double>;
