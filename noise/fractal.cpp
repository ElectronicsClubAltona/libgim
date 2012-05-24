/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
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


util::noise::fractal::fractal ()
{ ;}


util::noise::fractal::~fractal ()
{ ; }


double
util::noise::fractal::eval (double, double) const
    { unreachable (); }


///////////////////////////////////////////////////////////////////////////////
template <typename B>
util::noise::fbm<B>::fbm ()
{ ; }


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


template struct util::noise::fbm<util::noise::cellular>;
template struct util::noise::fbm<util::noise::gradient<lerp::linear>>;
template struct util::noise::fbm<util::noise::gradient<lerp::quintic>>;
template struct util::noise::fbm<util::noise::value<lerp::linear>>;
template struct util::noise::fbm<util::noise::value<lerp::quintic>>;


///////////////////////////////////////////////////////////////////////////////
template <typename B>
util::noise::musgrave<B>::musgrave ()
{ ; }


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

template struct util::noise::musgrave<util::noise::cellular>;
template struct util::noise::musgrave<util::noise::gradient<lerp::linear>>;
template struct util::noise::musgrave<util::noise::gradient<lerp::quintic>>;
template struct util::noise::musgrave<util::noise::value<lerp::linear>>;
template struct util::noise::musgrave<util::noise::value<lerp::quintic>>;

