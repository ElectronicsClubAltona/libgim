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

#include "noise/basis.hpp"
#include "noise/lut.hpp"

#include "../vector.hpp"
#include "../point.hpp"
#include "../random.hpp"

#include <algorithm>

using namespace util::noise;

///////////////////////////////////////////////////////////////////////////////
// Generate a type from [-UNIT..UNIT]
template <typename T>
T
generate (intmax_t x, intmax_t y, basis::seed_t);


template <>
double
generate (intmax_t x, intmax_t y, basis::seed_t seed) {
    size_t idx = permute (x, y, seed);
    return LUT[idx];
}


template <>
util::vector2
generate (intmax_t x, intmax_t y, basis::seed_t seed) {
    auto u = permute (x, y, seed);
    auto v = permute (u ^ seed);

    return util::vector2 (LUT[u], LUT[v]);
}


///////////////////////////////////////////////////////////////////////////////
basis::basis (seed_t _seed):
    seed (_seed)
{ ; }


basis::basis ():
    seed (util::random<seed_t> ())
{ ; }


basis::~basis ()
{ ; }


double
basis::eval (double, double) const
{ unreachable (); }



///////////////////////////////////////////////////////////////////////////////
template <lerp_function L>
value<L>::value (seed_t _seed):
    basis (_seed)
{ ; }


template <lerp_function L>
    value<L>::value ()
{ ; }


template <lerp_function L>
double 
value<L>::eval (double x, double y) const {
    intmax_t x_int = static_cast<intmax_t> (x);
    intmax_t y_int = static_cast<intmax_t> (y);
    double   x_fac = x - x_int;
    double   y_fac = y - y_int;

    // Generate the four corner values
    double p0 = generate<double> (x_int,     y_int,     this->seed);
    double p1 = generate<double> (x_int + 1, y_int,     this->seed);
    double p2 = generate<double> (x_int,     y_int + 1, this->seed);
    double p3 = generate<double> (x_int + 1, y_int + 1, this->seed);

    // Interpolate on one dimension, then the other.
    return L (L (p0, p1, x_fac),
              L (p2, p3, x_fac),
              y_fac);
}

template struct value<lerp::linear>;
template struct value<lerp::cubic>;
template struct value<lerp::quintic>;


///////////////////////////////////////////////////////////////////////////////
template <lerp_function L>
gradient<L>::gradient (seed_t _seed):
    basis (_seed)
{ ; }

template <lerp_function L>
gradient<L>::gradient ()
{ ; }


template <lerp_function L>
double 
gradient<L>::eval (double x, double y) const {
    intmax_t x_int = static_cast<intmax_t> (x);
    intmax_t y_int = static_cast<intmax_t> (y);
    double   x_fac = x - x_int;
    double   y_fac = y - y_int;

    // Generate the four corner values
    vector2 p0 = generate<vector2> (x_int,     y_int,       this->seed);
    vector2 p1 = generate<vector2> (x_int + 1, y_int,       this->seed);
    vector2 p2 = generate<vector2> (x_int,     y_int + 1,   this->seed);
    vector2 p3 = generate<vector2> (x_int + 1, y_int + 1,   this->seed);

    double  v0 = p0.x *        x_fac  + p0.y *  y_fac;
    double  v1 = p1.x * (x_fac - 1.0) + p1.y *  y_fac;
    double  v2 = p2.x *        x_fac  + p2.y * (y_fac - 1.0);
    double  v3 = p3.x * (x_fac - 1.0) + p3.y * (y_fac - 1.0);

    return L (L (v0, v1, x_fac),
              L (v2, v3, x_fac),
              y_fac);
}

template struct gradient<lerp::linear>;
template struct gradient<lerp::cubic>;
template struct gradient<lerp::quintic>;


///////////////////////////////////////////////////////////////////////////////
cellular::cellular (seed_t _seed):
    basis (_seed)
{ ; }


cellular::cellular ()
{ ; }


double
cellular::eval (double x, double y) const {
    using util::point2;

    intmax_t x_int = static_cast<intmax_t> (x);
    intmax_t y_int = static_cast<intmax_t> (y);
    double   x_fac = x - x_int;
    double   y_fac = y - y_int;

    // +---+---+---+
    // | 0 | 1 | 2 |
    // +---+---+---+
    // | 3 | 4 | 5 |
    // +---+-------+
    // | 6 | 7 | 8 |
    // +---+---+---+

    point2 centre = { x_fac, y_fac };
    double distances[9] = { std::numeric_limits<double>::quiet_NaN () };
    double *cursor = distances;

    for (signed y_off = -1; y_off <= 1 ; ++y_off)
        for (signed x_off = -1; x_off <= 1; ++x_off) {
            auto pos = point2 (double (x_off), double (y_off));
            auto off = generate<vector2> (x_int + x_off, y_int + y_off, this->seed);
            off += 1;
            off /= 2;

            CHECK (off.x >= 0 && off.x <= 1.0);
            CHECK (off.y >= 0 && off.y <= 1.0);

            pos += off;
            *cursor++ = pos.distance2 (centre);
        }

    std::sort (std::begin (distances), std::end (distances));
    return distances[0];
}
