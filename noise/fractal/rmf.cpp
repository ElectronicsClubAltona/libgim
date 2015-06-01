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

#include "rmf.hpp"

#include "../basis/constant.hpp"
#include "../basis/perlin.hpp"
#include "../basis/value.hpp"
#include "../basis/worley.hpp"
#include "../lerp.hpp"

using util::noise::fractal::rmf;


///////////////////////////////////////////////////////////////////////////////
template <typename T, typename B>
rmf<T,B>::rmf (unsigned _octaves,
                         T        _frequency,
                         T        _lacunarity,
                         T        _amplitude,
                         T        _gain,
                         seed_t   _seed):
    seed (_seed),
    octaves (_octaves),
    frequency (_frequency),
    lacunarity (_lacunarity),
    amplitude (_amplitude),
    gain (_gain),
    basis (_seed)
{ ; }


//-----------------------------------------------------------------------------
template <typename T, typename B>
rmf<T,B>::rmf ():
    rmf<T,B> (DEFAULT_OCTAVES,
              DEFAULT_FREQUENCY,
              DEFAULT_LACUNARITY,
              DEFAULT_AMPLITUDE,
              DEFAULT_GAIN,
              rand ())
{ ; }


//-----------------------------------------------------------------------------
template <typename T, typename B>
T
rmf<T,B>::operator() (util::point<2,T> p) const {
    const T offset = 1;
    const T H = 1.f;

    T exponents[octaves];
    for (size_t i = 0; i < octaves; ++i)
        exponents[i] = std::pow (std::pow (lacunarity, float (i)), -H);

    T signal = 0;
    T result = 0;
    T weight = 1;

    p *= frequency;

    for (size_t i = 0; i < octaves; ++i) {
        // generates ridged noise
        signal = basis (p);
        signal = std::fabs (signal);
        signal = offset - signal;

        // sharpens the ridges
        signal *= signal;

        // influence by sharpness of previous iteration
        signal *= weight;

        // contribute to the weight
        weight = signal * gain;
        weight = limit (weight, 0, 1);

        // record and continue
        result += signal * exponents[i];

        p *= lacunarity;
    }

    return result;
}


//-----------------------------------------------------------------------------
template struct util::noise::fractal::rmf<float, util::noise::basis::worley<float>>;
template struct util::noise::fractal::rmf<float, util::noise::basis::perlin<float,util::lerp::linear>>;
template struct util::noise::fractal::rmf<float, util::noise::basis::perlin<float,util::lerp::cubic>>;
template struct util::noise::fractal::rmf<float, util::noise::basis::perlin<float,util::lerp::quintic>>;
template struct util::noise::fractal::rmf<float, util::noise::basis::value<float,util::lerp::linear>>;
template struct util::noise::fractal::rmf<float, util::noise::basis::value<float,util::lerp::quintic>>;

template struct util::noise::fractal::rmf<double, util::noise::basis::worley<double>>;
template struct util::noise::fractal::rmf<double, util::noise::basis::perlin<double,util::lerp::linear>>;
template struct util::noise::fractal::rmf<double, util::noise::basis::perlin<double,util::lerp::quintic>>;
template struct util::noise::fractal::rmf<double, util::noise::basis::value<double,util::lerp::linear>>;
template struct util::noise::fractal::rmf<double, util::noise::basis::value<double,util::lerp::quintic>>;
