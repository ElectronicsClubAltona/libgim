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


using util::noise::fractal;
using util::noise::fbm;
using util::noise::musgrave;


///////////////////////////////////////////////////////////////////////////////
template <typename T>
fractal<T>::fractal (unsigned _octaves,
                     T        _frequency,
                     T        _lacunarity):
    octaves (_octaves),
    frequency (_frequency),
    lacunarity (_lacunarity)
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
fractal<T>::fractal ():
    octaves    (1),
    frequency  (T(0.1)),
    lacunarity (T(0.6))
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
               seed_t   _seed):
    fractal<T> (_octaves, _frequency, _lacunarity),
    basis (_seed)
{ ; }


//-----------------------------------------------------------------------------
template <typename T, typename B>
fbm<T,B>::fbm ()
{ ; }


//-----------------------------------------------------------------------------
template <typename T, typename B>
T
fbm<T,B>::operator() (T x, T y) const {
    T total = 0;
    T f     = this->frequency;
    T a     = 1;
    T a_sum = 0;

    for (size_t i = 0; i < this->octaves; ++i) {
        total += basis (x * f, y * f) * a;

        f *= 2;

        a_sum += a;
        a *= this->lacunarity;
    }

    return total;
}


//-----------------------------------------------------------------------------
template struct util::noise::fbm<float, util::noise::cellular<float>>;
template struct util::noise::fbm<float, util::noise::gradient<float,util::lerp::linear>>;
template struct util::noise::fbm<float, util::noise::gradient<float,util::lerp::quintic>>;
template struct util::noise::fbm<float, util::noise::value<float,util::lerp::linear>>;
template struct util::noise::fbm<float, util::noise::value<float,util::lerp::quintic>>;

template struct util::noise::fbm<double, util::noise::cellular<double>>;
template struct util::noise::fbm<double, util::noise::gradient<double,util::lerp::linear>>;
template struct util::noise::fbm<double, util::noise::gradient<double,util::lerp::quintic>>;
template struct util::noise::fbm<double, util::noise::value<double,util::lerp::linear>>;
template struct util::noise::fbm<double, util::noise::value<double,util::lerp::quintic>>;


///////////////////////////////////////////////////////////////////////////////
template <typename T, typename B>
musgrave<T,B>::musgrave (unsigned _octaves,
                         T        _frequency,
                         T        _lacunarity,
                         seed_t   _seed):
    fractal<T> (_octaves, _frequency, _lacunarity),
    basis (_seed)
{ ; }


//-----------------------------------------------------------------------------
template <typename T, typename B>
musgrave<T,B>::musgrave ()
{ ; }


//-----------------------------------------------------------------------------
template <typename T, typename B>
T
musgrave<T,B>::operator() (T x, T y) const {
    T total     = 0;
    T f         = this->frequency;
    T a         = 1;

    T weight    = 1;
    T offset    = 1;
    T gain      = 2;

    T signal;

    signal  = basis (x * f, y * f);
    signal  = std::fabs (signal);
    signal  = offset - signal;
    signal *= signal;
    total   = signal;

    for (size_t i = 1; i < this->octaves; ++i) {
        f *= 2;
        a *= this->lacunarity;

        weight = signal * gain;
        weight = limit (weight, 0, 1);

        signal = basis (x * f, y * f);
        signal = std::fabs (signal);
        signal = offset - signal;
        signal *= signal;

        signal *= weight;
        total += signal * a;
        total  = limit (total, 0, 1);
    }

    return total;
}


//-----------------------------------------------------------------------------
template struct util::noise::musgrave<float, util::noise::cellular<float>>;
template struct util::noise::musgrave<float, util::noise::gradient<float,util::lerp::linear>>;
template struct util::noise::musgrave<float, util::noise::gradient<float,util::lerp::quintic>>;
template struct util::noise::musgrave<float, util::noise::value<float,util::lerp::linear>>;
template struct util::noise::musgrave<float, util::noise::value<float,util::lerp::quintic>>;

template struct util::noise::musgrave<double, util::noise::cellular<double>>;
template struct util::noise::musgrave<double, util::noise::gradient<double,util::lerp::linear>>;
template struct util::noise::musgrave<double, util::noise::gradient<double,util::lerp::quintic>>;
template struct util::noise::musgrave<double, util::noise::value<double,util::lerp::linear>>;
template struct util::noise::musgrave<double, util::noise::value<double,util::lerp::quintic>>;

