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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#include "lerp.hpp"

#include "debug.hpp"
#include "maths.hpp"

#include <cmath>


double
lerp::sigmoid (double val) {
    return -1.0 + 2.0 / (1.0 + exp (-2.0 * val));
}


double lerp::trunc (double a, double, double)
    { return a; }


double
lerp::linear (double a, double b, double weight) {
    CHECK (weight >= 0.0 && weight <= 1.0);
    return a * (1.0 - weight) + b * weight;
}


double
lerp::cosine (double a, double b, double weight) {
    CHECK (weight >= 0.0 && weight <= 1.0);
    double t  = (1.0 - cos (weight * PI_d)) * 0.5;

    return a * (1.0 - t) + b * t;
}


double
lerp::cubic (double a, double b, double weight) {
    CHECK (weight >= 0.0 && weight <= 1.0);
    double t = weight * weight * (3.0 - 2.0 * weight);
    return a * (1.0 - t) + b * t;
}


double
lerp::quintic (double a, double b, double weight) {
    CHECK (weight >= 0.0 && weight <= 1.0);
    double t = weight * weight * weight * (weight * (weight * 6.0 - 15.0) + 10.0);
    return a * (1.0 - t) + b * t;
}
