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
 * Copyright 2012-2015 Danny Robson <danny@nerdcruft.net>
 */

#include "hetero.hpp"

#include "../basis/constant.hpp"
#include "../basis/perlin.hpp"
#include "../basis/value.hpp"
#include "../basis/worley.hpp"
#include "../lerp.hpp"

using util::noise::fractal::hetero;


//-----------------------------------------------------------------------------
template <typename T, typename B>
hetero<T,B>::hetero():
    H (0.75f),
    octaves (6),
    frequency (0.1f),
    lacunarity (2),
    offset (0.7f),
    amplitude (1),
    gain (1)
{ ; }


//-----------------------------------------------------------------------------
template <typename T, typename B>
T
hetero<T,B>::operator() (util::point<2,T> p) const
{
    T exponents[octaves];
    for (size_t i = 0; i < octaves; ++i)
        exponents[i] = std::pow (std::pow (lacunarity, float (i)), -H);

    T result = 0;
    T increment = 0;

    p *= frequency;
    result = basis (p) + offset;
    p *= lacunarity;

    for (size_t i = 0; i < octaves; ++i) {
        increment  = basis (p) + offset;
        increment *= exponents[i];
        increment *= result;

        result += increment;

        p *= lacunarity;
    }

    return result;
}


//-----------------------------------------------------------------------------
template struct util::noise::fractal::hetero<float, util::noise::basis::worley<float>>;
template struct util::noise::fractal::hetero<float, util::noise::basis::perlin<float,util::lerp::linear>>;
template struct util::noise::fractal::hetero<float, util::noise::basis::perlin<float,util::lerp::quintic>>;
template struct util::noise::fractal::hetero<float, util::noise::basis::value<float,util::lerp::linear>>;
template struct util::noise::fractal::hetero<float, util::noise::basis::value<float,util::lerp::quintic>>;
