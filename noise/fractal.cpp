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

#include "noise/fractal.hpp"

#include "debug.hpp"

#include <algorithm>
#include <cmath>


using util::noise::fractal;
using util::noise::fbm;
using util::noise::rmf;


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
fractal<T>::operator() (T, T) const
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
    fbm<T,B> (8, T(0.1), 2, 1, 1/T(2), rand ())
{ ; }


//-----------------------------------------------------------------------------
template <typename T, typename B>
T
fbm<T,B>::operator() (T x, T y) const {
    T total = 0;
    T f = frequency;
    T a = amplitude;

    for (size_t i = 0; i < octaves; ++i) {
        total += basis (x * f, y * f) * a;

        f *= lacunarity;
        a *= gain;
    }

    return total;
}


//-----------------------------------------------------------------------------
template struct util::noise::fbm<float, util::noise::cellular<float>>;
template struct util::noise::fbm<float, util::noise::gradient<float,util::lerp::linear>>;
template struct util::noise::fbm<float, util::noise::gradient<float,util::lerp::quintic>>;
template struct util::noise::fbm<float, util::noise::value<float,util::lerp::trunc>>;
template struct util::noise::fbm<float, util::noise::value<float,util::lerp::linear>>;
template struct util::noise::fbm<float, util::noise::value<float,util::lerp::quintic>>;

template struct util::noise::fbm<double, util::noise::cellular<double>>;
template struct util::noise::fbm<double, util::noise::gradient<double,util::lerp::linear>>;
template struct util::noise::fbm<double, util::noise::gradient<double,util::lerp::quintic>>;
template struct util::noise::fbm<double, util::noise::value<double,util::lerp::trunc>>;
template struct util::noise::fbm<double, util::noise::value<double,util::lerp::linear>>;
template struct util::noise::fbm<double, util::noise::value<double,util::lerp::quintic>>;


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
    rmf<T,B> (6, T(1), 2, 1, T(2), rand ())
{ ; }


//-----------------------------------------------------------------------------
template <typename T, typename B>
T
rmf<T,B>::operator() (T x, T y) const {
    T H = 1;
    T exponents[octaves];
    T offset = 1;

    T f = 1.f;
    for (size_t i = 0; i < octaves; ++i) {
        exponents[i] = std::pow (f, -H);
        f *= lacunarity;
    }

    T signal = 0, result = 0, weight = 1;

    x *= frequency;
    y *= frequency;

    for (size_t i = 0; i < octaves; ++i) {
        signal = basis (x, y);
        signal = std::fabs (signal);
        signal = offset - signal;

        signal *= signal;
        signal *= weight;

        weight = signal * gain;
        weight = limit (weight, 0, 1);

        result += signal * exponents[i];

        x *= lacunarity;
        y *= lacunarity;
    }

    return result;
}


//-----------------------------------------------------------------------------
template struct util::noise::rmf<float, util::noise::cellular<float>>;
template struct util::noise::rmf<float, util::noise::gradient<float,util::lerp::linear>>;
template struct util::noise::rmf<float, util::noise::gradient<float,util::lerp::quintic>>;
template struct util::noise::rmf<float, util::noise::value<float,util::lerp::linear>>;
template struct util::noise::rmf<float, util::noise::value<float,util::lerp::quintic>>;

template struct util::noise::rmf<double, util::noise::cellular<double>>;
template struct util::noise::rmf<double, util::noise::gradient<double,util::lerp::linear>>;
template struct util::noise::rmf<double, util::noise::gradient<double,util::lerp::quintic>>;
template struct util::noise::rmf<double, util::noise::value<double,util::lerp::linear>>;
template struct util::noise::rmf<double, util::noise::value<double,util::lerp::quintic>>;

