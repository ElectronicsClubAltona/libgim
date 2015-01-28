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

#include "../debug.hpp"
#include "../point.hpp"
#include "../random.hpp"
#include "../vector.hpp"

#include <algorithm>

using namespace util::noise;
using util::range;

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
util::vector2d
generate (intmax_t x, intmax_t y, basis::seed_t seed) {
    auto u = permute (x, y, seed);
    auto v = permute (u ^ seed);

    return util::vector2d (LUT[u], LUT[v]);
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
range<double>
value<L>::bounds (void) const
    { return { -1.0, 1.0 }; }


template <lerp_function L>
double 
value<L>::eval (double x, double y) const {
    intmax_t x_int = static_cast<intmax_t> (x);
    intmax_t y_int = static_cast<intmax_t> (y);
    double   x_fac = x - x_int;
    double   y_fac = y - y_int;

    // Shift the coordinate system down a little to ensure we get unit weights
    // for the lerp. It's better to do this than abs the fractional portion so
    // we don't get reflections along the origin.
    if (x < 0) { x_fac = 1.0 + x_fac; x_int -= 1; }
    if (y < 0) { y_fac = 1.0 + y_fac; y_int -= 1; }

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


namespace util {
    namespace noise {
        template struct value<lerp::linear>;
        template struct value<lerp::cubic>;
        template struct value<lerp::quintic>;
    }
}


///////////////////////////////////////////////////////////////////////////////
template <lerp_function L>
gradient<L>::gradient (seed_t _seed):
    basis (_seed)
{ ; }


template <lerp_function L>
gradient<L>::gradient ()
{ ; }


template <lerp_function L>
range<double>
gradient<L>::bounds (void) const
    { return { -sqrt(2.0) / 2.0, sqrt (2.0) / 2.0 }; }


template <lerp_function L>
double 
gradient<L>::eval (double x, double y) const {
    intmax_t x_int = static_cast<intmax_t> (x);
    intmax_t y_int = static_cast<intmax_t> (y);
    double   x_fac = x - x_int;
    double   y_fac = y - y_int;

    // Shift the coordinate system down a little to ensure we get unit weights
    // for the lerp. It's better to do this than abs the fractional portion so
    // we don't get reflections along the origin.
    if (x < 0) { x_fac = 1.0 + x_fac; x_int -= 1; }
    if (y < 0) { y_fac = 1.0 + y_fac; y_int -= 1; }

    // Generate the four corner values. It's not strictly necessary to
    // normalise the values, but we get a more consistent and visually
    // appealing range of outputs with normalised values.
    vector2d p0 = generate<vector2d> (x_int,     y_int,       this->seed).normalise ();
    vector2d p1 = generate<vector2d> (x_int + 1, y_int,       this->seed).normalise ();
    vector2d p2 = generate<vector2d> (x_int,     y_int + 1,   this->seed).normalise ();
    vector2d p3 = generate<vector2d> (x_int + 1, y_int + 1,   this->seed).normalise ();

    double  v0 = p0.x *        x_fac  + p0.y *  y_fac;
    double  v1 = p1.x * (x_fac - 1.0) + p1.y *  y_fac;
    double  v2 = p2.x *        x_fac  + p2.y * (y_fac - 1.0);
    double  v3 = p3.x * (x_fac - 1.0) + p3.y * (y_fac - 1.0);

    return L (L (v0, v1, x_fac),
              L (v2, v3, x_fac),
              y_fac);
}


namespace util {
    namespace noise {
        template struct gradient<lerp::linear>;
        template struct gradient<lerp::cubic>;
        template struct gradient<lerp::quintic>;
    }
}


///////////////////////////////////////////////////////////////////////////////
cellular::cellular (seed_t _seed):
    basis (_seed)
{ ; }


cellular::cellular ()
{ ; }


range<double>
cellular::bounds (void) const
    { return { 0.0, 1.5 }; }


double
cellular::eval (double x, double y) const {
    using util::point2d;

    intmax_t x_int = static_cast<intmax_t> (x);
    intmax_t y_int = static_cast<intmax_t> (y);
    double   x_fac = x - x_int;
    double   y_fac = y - y_int;

    // Generate the four corner values. It's not strictly necessary to
    // normalise the values, but we get a more consistent and visually
    // appealing range of outputs with normalised values.
    if (x < 0) { x_fac = 1.0 + x_fac; x_int -= 1; }
    if (y < 0) { y_fac = 1.0 + y_fac; y_int -= 1; }

    // +---+---+---+
    // | 0 | 1 | 2 |
    // +---+---+---+
    // | 3 | 4 | 5 |
    // +---+-------+
    // | 6 | 7 | 8 |
    // +---+---+---+

    point2d centre = { x_fac, y_fac };
    double distances[9] = { std::numeric_limits<double>::quiet_NaN () };
    double *cursor = distances;

    for (signed y_off = -1; y_off <= 1 ; ++y_off)
        for (signed x_off = -1; x_off <= 1; ++x_off) {
            auto pos = point2d (double (x_off), double (y_off));
            auto off = generate<vector2d> (x_int + x_off, y_int + y_off, this->seed);
            off += 1;
            off /= 2;

            CHECK (off.x >= 0 && off.x <= 1.0);
            CHECK (off.y >= 0 && off.y <= 1.0);

            pos += off;
            *cursor++ = pos.distance2 (centre);
        }

    std::sort (std::begin (distances), std::end (distances));
    CHECK_GE (distances[0], 0);
    CHECK (bounds ().contains (distances[0]));
    return distances[0];
}
