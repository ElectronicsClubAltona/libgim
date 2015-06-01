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

#include "fbm.hpp"

#include "../basis/constant.hpp"
#include "../basis/perlin.hpp"
#include "../basis/value.hpp"
#include "../basis/worley.hpp"
#include "../lerp.hpp"

using util::noise::fractal::fbm;


///////////////////////////////////////////////////////////////////////////////
template <typename T, typename B>
fbm<T,B>::fbm (unsigned _octaves,
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
{
    CHECK_NEQ (octaves, 0);
    CHECK_NEQ (frequency, 0);
    CHECK_NEQ (amplitude, 0);
}


//-----------------------------------------------------------------------------
template <typename T, typename B>
fbm<T,B>::fbm ():
    fbm<T,B> (DEFAULT_OCTAVES,
              DEFAULT_FREQUENCY,
              DEFAULT_LACUNARITY,
              DEFAULT_AMPLITUDE,
              DEFAULT_GAIN,
              rand ())
{ ; }


//-----------------------------------------------------------------------------
template <typename T, typename B>
T
fbm<T,B>::operator() (util::point<2,T> p) const {
    T total = 0;
    T f = frequency;
    T a = amplitude;

    for (size_t i = 0; i < octaves; ++i) {
        total += basis (p * f) * a;

        f *= lacunarity;
        a *= gain;
    }

    return total;
}


//-----------------------------------------------------------------------------
template struct util::noise::fractal::fbm<float, util::noise::basis::worley<float>>;
template struct util::noise::fractal::fbm<float, util::noise::basis::constant<float>>;
template struct util::noise::fractal::fbm<float, util::noise::basis::perlin<float,util::lerp::trunc>>;
template struct util::noise::fractal::fbm<float, util::noise::basis::perlin<float,util::lerp::linear>>;
template struct util::noise::fractal::fbm<float, util::noise::basis::perlin<float,util::lerp::cubic>>;
template struct util::noise::fractal::fbm<float, util::noise::basis::perlin<float,util::lerp::quintic>>;
template struct util::noise::fractal::fbm<float, util::noise::basis::value<float,util::lerp::trunc>>;
template struct util::noise::fractal::fbm<float, util::noise::basis::value<float,util::lerp::linear>>;
template struct util::noise::fractal::fbm<float, util::noise::basis::value<float,util::lerp::cosine>>;
template struct util::noise::fractal::fbm<float, util::noise::basis::value<float,util::lerp::cubic>>;
template struct util::noise::fractal::fbm<float, util::noise::basis::value<float,util::lerp::quintic>>;

template struct util::noise::fractal::fbm<double, util::noise::basis::worley<double>>;
template struct util::noise::fractal::fbm<double, util::noise::basis::perlin<double,util::lerp::linear>>;
template struct util::noise::fractal::fbm<double, util::noise::basis::perlin<double,util::lerp::quintic>>;
template struct util::noise::fractal::fbm<double, util::noise::basis::value<double,util::lerp::trunc>>;
template struct util::noise::fractal::fbm<double, util::noise::basis::value<double,util::lerp::linear>>;
template struct util::noise::fractal::fbm<double, util::noise::basis::value<double,util::lerp::quintic>>;

