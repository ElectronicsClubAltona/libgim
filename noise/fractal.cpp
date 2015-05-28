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
 * Copyright 2012 Danny Robson <danny@nerdcruft.net>
 */

#include "fractal.hpp"

#include "basis/value.hpp"
#include "basis/perlin.hpp"
#include "basis/worley.hpp"
#include "lerp.hpp"

#include "../debug.hpp"

#include <algorithm>
#include <cmath>


using util::noise::fractal;
using util::noise::fbm;
using util::noise::rmf;
using util::noise::hmf;
using util::noise::hetero;


///////////////////////////////////////////////////////////////////////////////
template <typename T>
fractal<T>::fractal ():
    seed (rand ())
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
fractal<T>::fractal (seed_t _seed):
    seed (_seed)
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
fractal<T>::~fractal ()
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
T
fractal<T>::operator() (util::point<2,T>) const
{
    unreachable ();
}


//-----------------------------------------------------------------------------
namespace util { namespace noise {
    template struct fractal<float>;
    template struct fractal<double>;
} }


///////////////////////////////////////////////////////////////////////////////
template <typename T, typename B>
fbm<T,B>::fbm (unsigned _octaves,
               T        _frequency,
               T        _lacunarity,
               T        _amplitude,
               T        _gain,
               seed_t   _seed):
    fractal<T> (_seed),
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
template struct util::noise::fbm<float, util::noise::basis::worley<float>>;
template struct util::noise::fbm<float, util::noise::basis::perlin<float,util::lerp::linear>>;
template struct util::noise::fbm<float, util::noise::basis::perlin<float,util::lerp::quintic>>;
template struct util::noise::fbm<float, util::noise::basis::value<float,util::lerp::trunc>>;
template struct util::noise::fbm<float, util::noise::basis::value<float,util::lerp::linear>>;
template struct util::noise::fbm<float, util::noise::basis::value<float,util::lerp::quintic>>;

template struct util::noise::fbm<double, util::noise::basis::worley<double>>;
template struct util::noise::fbm<double, util::noise::basis::perlin<double,util::lerp::linear>>;
template struct util::noise::fbm<double, util::noise::basis::perlin<double,util::lerp::quintic>>;
template struct util::noise::fbm<double, util::noise::basis::value<double,util::lerp::trunc>>;
template struct util::noise::fbm<double, util::noise::basis::value<double,util::lerp::linear>>;
template struct util::noise::fbm<double, util::noise::basis::value<double,util::lerp::quintic>>;


///////////////////////////////////////////////////////////////////////////////
template <typename T, typename B>
rmf<T,B>::rmf (unsigned _octaves,
                         T        _frequency,
                         T        _lacunarity,
                         T        _amplitude,
                         T        _gain,
                         seed_t   _seed):
    fractal<T> (_seed),
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
template struct util::noise::rmf<float, util::noise::basis::worley<float>>;
template struct util::noise::rmf<float, util::noise::basis::perlin<float,util::lerp::linear>>;
template struct util::noise::rmf<float, util::noise::basis::perlin<float,util::lerp::quintic>>;
template struct util::noise::rmf<float, util::noise::basis::value<float,util::lerp::linear>>;
template struct util::noise::rmf<float, util::noise::basis::value<float,util::lerp::quintic>>;

template struct util::noise::rmf<double, util::noise::basis::worley<double>>;
template struct util::noise::rmf<double, util::noise::basis::perlin<double,util::lerp::linear>>;
template struct util::noise::rmf<double, util::noise::basis::perlin<double,util::lerp::quintic>>;
template struct util::noise::rmf<double, util::noise::basis::value<double,util::lerp::linear>>;
template struct util::noise::rmf<double, util::noise::basis::value<double,util::lerp::quintic>>;


//-----------------------------------------------------------------------------
template <typename T, typename B>
hmf<T,B>::hmf ():
    H (0.25f),
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
hmf<T,B>::operator() (util::point<2,T> p) const
{
    T exponents[octaves];
    for (size_t i = 0; i < octaves; ++i)
        exponents[i] = std::pow (std::pow (lacunarity, float (i)), -H);

    T result = 0;
    T signal = 0;
    T weight = 1;

    p *= frequency;

    for (size_t i = 0; i < octaves; ++i) {
        signal = (basis (p) + offset) * exponents[i];
        result += weight * signal;

        weight *= gain * signal;
        if (weight > 1)
            weight = 1;

        p *= lacunarity;
    }

    return result;
}


template struct util::noise::hmf<float, util::noise::basis::worley<float>>;
template struct util::noise::hmf<float, util::noise::basis::perlin<float,util::lerp::linear>>;
template struct util::noise::hmf<float, util::noise::basis::perlin<float,util::lerp::quintic>>;
template struct util::noise::hmf<float, util::noise::basis::value<float,util::lerp::linear>>;
template struct util::noise::hmf<float, util::noise::basis::value<float,util::lerp::quintic>>;


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


template struct util::noise::hetero<float, util::noise::basis::worley<float>>;
template struct util::noise::hetero<float, util::noise::basis::perlin<float,util::lerp::linear>>;
template struct util::noise::hetero<float, util::noise::basis::perlin<float,util::lerp::quintic>>;
template struct util::noise::hetero<float, util::noise::basis::value<float,util::lerp::linear>>;
template struct util::noise::hetero<float, util::noise::basis::value<float,util::lerp::quintic>>;
