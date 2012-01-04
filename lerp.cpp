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
 * Copyright 2011 Danny Robson <danny@blubinc.net>
 */

#include "lerp.hpp"

#include "debug.hpp"
#include "maths.hpp"

#include <cmath>


double
lerp::sigmoid (double val) {
    return -1.0 + 2.0 / (1.0 + exp (-2.0 * val));
}


double
lerp::linear (double a, double b, double weight) {
    check (weight >= 0 && weight <= 1.0);
    return (1.0 - weight) * a + weight * b;
}


double
lerp::cosine (double a, double b, double weight) {
    check (weight >= 0 && weight <= 1.0);
    double ft = weight * PI;
    double f  = (1.0 - cos (ft)) * 0.5;

    return a * (1.0 - f) + b * f;
}

