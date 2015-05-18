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


///////////////////////////////////////////////////////////////////////////////
util::noise::fractal::fractal (unsigned      _octaves,
                               double        _frequency,
                               double        _lacunarity):
    octaves (_octaves),
    frequency (_frequency),
    lacunarity (_lacunarity)
{ ; }


//-----------------------------------------------------------------------------
util::noise::fractal::fractal ():
    octaves    (1),
    frequency  (0.1),
    lacunarity (0.6)
{ ; }


//-----------------------------------------------------------------------------
util::noise::fractal::~fractal ()
{ ; }


//-----------------------------------------------------------------------------
double
util::noise::fractal::eval (double, double) const
    { unreachable (); }


///////////////////////////////////////////////////////////////////////////////
template <typename B>
util::noise::fbm<B>::fbm (unsigned _octaves,
                          double   _frequency,
                          double   _lacunarity,
                          seed_t   _seed):
    fractal (_octaves, _frequency, _lacunarity),
    basis (_seed)
{ ; }


//-----------------------------------------------------------------------------
template <typename B>
util::noise::fbm<B>::fbm ()
{ ; }


//-----------------------------------------------------------------------------
template <typename B>
double
util::noise::fbm<B>::eval (double x, double y) const {
    double total = 0.0;
    double f     = this->frequency;
    double a     = 1.0;
    double a_sum = 0.0;

    for (size_t i = 0; i < this->octaves; ++i) {
        total += basis.eval (x * f, y * f) * a;

        f *= 2.0;

        a_sum += a;
        a *= this->lacunarity;
    }

    return total;
}


//-----------------------------------------------------------------------------
template struct util::noise::fbm<util::noise::cellular<double>>;
template struct util::noise::fbm<util::noise::gradient<double,util::lerp::linear>>;
template struct util::noise::fbm<util::noise::gradient<double,util::lerp::quintic>>;
template struct util::noise::fbm<util::noise::value<double,util::lerp::linear>>;
template struct util::noise::fbm<util::noise::value<double,util::lerp::quintic>>;


///////////////////////////////////////////////////////////////////////////////
template <typename B>
util::noise::musgrave<B>::musgrave (unsigned _octaves,
                                    double   _frequency,
                                    double   _lacunarity,
                                    seed_t   _seed):
    fractal (_octaves, _frequency, _lacunarity),
    basis (_seed)
{ ; }


//-----------------------------------------------------------------------------
template <typename B>
util::noise::musgrave<B>::musgrave ()
{ ; }


//-----------------------------------------------------------------------------
template <typename B>
double
util::noise::musgrave<B>::eval (double x, double y) const {
    double total     = 0.0;
    double f         = this->frequency;
    double a         = 1.0;

    double weight    = 1.0;
    double offset    = 1.0;
    double gain      = 2.0;

    double signal;

    signal  = basis.eval (x * f, y * f);
    signal  = fabs (signal);
    signal  = offset - signal;
    signal *= signal;
    total   = signal;

    for (size_t i = 1; i < this->octaves; ++i) {
        f         *= 2.0;
        a         *= this->lacunarity;

        weight = signal * gain;
        weight = max (0.0, min (1.0, weight));

        signal = basis.eval (x * f, y * f);
        signal = fabs (signal);
        signal = offset - signal;
        signal *= signal;

        signal *= weight;
        total += signal * a;
        total  = min (1.0, max (0.0, total));
    }

    return total;
}


//-----------------------------------------------------------------------------
template struct util::noise::musgrave<util::noise::cellular<double>>;
template struct util::noise::musgrave<util::noise::gradient<double,util::lerp::linear>>;
template struct util::noise::musgrave<util::noise::gradient<double,util::lerp::quintic>>;
template struct util::noise::musgrave<util::noise::value<double,util::lerp::linear>>;
template struct util::noise::musgrave<util::noise::value<double,util::lerp::quintic>>;

