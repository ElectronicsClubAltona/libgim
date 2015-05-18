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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#include "lerp.hpp"

#include "debug.hpp"
#include "maths.hpp"

#include <cmath>


//-----------------------------------------------------------------------------
double
util::lerp::sigmoid (double val) {
    return -1.0 + 2.0 / (1.0 + exp (-2.0 * val));
}


//-----------------------------------------------------------------------------
double util::lerp::trunc (double a, double, double)
    { return a; }


//-----------------------------------------------------------------------------
double
util::lerp::linear (double a, double b, double weight) {
    CHECK (weight >= 0.0 && weight <= 1.0);
    return a * (1.0 - weight) + b * weight;
}


//-----------------------------------------------------------------------------
double
util::lerp::cosine (double a, double b, double weight) {
    CHECK (weight >= 0.0 && weight <= 1.0);
    double t  = (1.0 - cos (weight * PI<double>)) * 0.5;

    return a * (1.0 - t) + b * t;
}


//-----------------------------------------------------------------------------
double
util::lerp::cubic (double a, double b, double weight) {
    CHECK (weight >= 0.0 && weight <= 1.0);
    double t = weight * weight * (3.0 - 2.0 * weight);
    return a * (1.0 - t) + b * t;
}


//-----------------------------------------------------------------------------
double
util::lerp::quintic (double a, double b, double weight) {
    CHECK (weight >= 0.0 && weight <= 1.0);
    double t = weight * weight * weight * (weight * (weight * 6.0 - 15.0) + 10.0);
    return a * (1.0 - t) + b * t;
}
