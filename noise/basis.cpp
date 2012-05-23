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
#include "../random.hpp"

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
    intmax_t x_int = intmax_t (x);
    double   x_fac = x - x_int;
    intmax_t y_int = intmax_t (y);
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
    intmax_t x_int = intmax_t (x);
    double   x_fac = x - x_int;
    intmax_t y_int = intmax_t (y);
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

