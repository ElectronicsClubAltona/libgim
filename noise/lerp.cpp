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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#include "lerp.hpp"

#include "debug.hpp"
#include "maths.hpp"

#include <cmath>


//-----------------------------------------------------------------------------
template <typename T>
T
util::lerp::sigmoid (T val)
{
    static_assert (std::is_floating_point<T>::value,
                   "lerp is only defined for floating types");
    return -1 + 2 / (1 + std::exp (-2 * val));
}

template float  util::lerp::sigmoid (float);
template double util::lerp::sigmoid (double);


//-----------------------------------------------------------------------------
template <typename T>
T
util::lerp::trunc (T a, T, T weight)
{
    static_assert (std::is_floating_point<T>::value,
                   "lerp is only defined for floating types");

    CHECK_LIMIT (weight, 0, 1);
    (void)weight;

    return a;
}

template float  util::lerp::trunc (float,  float,  float);
template double util::lerp::trunc (double, double, double);


//-----------------------------------------------------------------------------
template <typename T>
T
util::lerp::linear (T a, T b, T weight)
{
    static_assert (std::is_floating_point<T>::value,
                   "lerp is only defined for floating types");
    CHECK_LIMIT (weight, 0, 1);

    return a * (1 - weight) + b * weight;
}

template float  util::lerp::linear (float,  float,  float);
template double util::lerp::linear (double, double, double);


//-----------------------------------------------------------------------------
template <typename T>
T
util::lerp::cosine (T a, T b, T weight)
{
    static_assert (std::is_floating_point<T>::value,
                   "lerp is only defined for floating types");
    CHECK_LIMIT (weight, 0, 1);

    T t = (1 - std::cos (weight * PI<T>)) * T(0.5);
    return a * (1 - t) + b * t;
}

template float  util::lerp::cosine (float,  float,  float);
template double util::lerp::cosine (double, double, double);


//-----------------------------------------------------------------------------
template <typename T>
T
util::lerp::cubic (T a, T b, T weight)
{
    static_assert (std::is_floating_point<T>::value,
                   "lerp is only defined for floating types");
    CHECK_LIMIT (weight, 0, 1);

    // -2w^3 * 3w^2
    T t = weight * weight * (3 - 2 * weight);
    return a * (1 - t) + b * t;
}

template float  util::lerp::cubic (float,  float,  float);
template double util::lerp::cubic (double, double, double);


//-----------------------------------------------------------------------------
template <typename T>
T
util::lerp::quintic (T a, T b, T weight)
{
    static_assert (std::is_floating_point<T>::value,
                   "lerp is only defined for floating types");
    CHECK_LIMIT (weight, 0, 1);

    // from perlin's improved noise: 6w^5 -15w^4 +10w^3
    T t = weight * weight * weight * (weight * (weight * 6 - 15) + 10);
    return a * (1 - t) + b * t;
}


template float  util::lerp::quintic (float,  float,  float);
template double util::lerp::quintic (double, double, double);
