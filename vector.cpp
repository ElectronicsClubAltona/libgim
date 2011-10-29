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

#include "vector.hpp"

#include "debug.hpp"
#include "maths.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>


using namespace util;

util::vector
util::vector::operator* (double rhs) const {
    return { rhs * x, rhs * y, rhs * z };
}


vector&
vector::operator*= (double rhs) {
    x *= rhs;
    y *= rhs;
    z *= rhs;

    return *this;
}


util::vector
util::vector::operator* (const vector &rhs) const {
    return { x * rhs.x, y * rhs.y, z * rhs.z };
}


util::vector&
util::vector::operator*= (const vector &rhs) {
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;

    return *this;
}


vector
vector::operator+ (const vector &rhs) const {
    return { x + rhs.x, y + rhs.y, z + rhs.z };
}


vector
vector::operator- (void) const {
    return { -x, -y, -z };
}


vector
vector::operator- (const vector &rhs) const
    { return { x - rhs.x, y - rhs.y, z - rhs.z }; }


vector&
vector::operator-= (const vector &rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;

    return *this;
}


vector&
vector::operator+= (const vector &rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;

    return *this;
}


vector&
vector::operator= (const vector &rhs) {
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;

    return *this;
}


bool
vector::operator== (const vector &rhs) const {
    return almost_equal (x, rhs.x) &&
           almost_equal (y, rhs.y) &&
           almost_equal (z, rhs.z);
}


double
vector::magnitude (void) const {
    return sqrt (x * x + y * y + z * z);
}


double
vector::magnitude2 (void) const {
    return x * x + y * y + z * z;
}


double
vector::dot (const vector &rhs) const {
    return x * rhs.x + y * rhs.y + z * rhs.z;
}


vector
vector::cross (const vector &rhs) const { 
    return { y * rhs.z - z * rhs.y,
             z * rhs.x - x * rhs.z,
             x * rhs.y - y * rhs.x };
}


vector&
vector::normalise (void) {
    double mag = magnitude ();

    x /= mag;
    y /= mag;
    z /= mag;

    return *this;
}


vector
vector::normalised (void) const {
    double mag = magnitude ();
    return { x / mag, y / mag, z / mag };
}


vector
vector::spherical_to_cartesian (const vector &s) {
    return {
        s.x * sin (s.y) * cos (s.z),
        s.x * sin (s.y) * sin (s.z),
        s.x * cos (s.y),
    };
}


vector
vector::cartesian_to_spherical (const vector &c) {
    double mag = c.magnitude ();

    return {
        mag,
        acos  (c.z / mag),
        atan2 (c.y, c.x)
    };
}


void
vector::sanity (void) const {
    check_soft (!std::isnan (x));
    check_soft (!std::isnan (y));
    check_soft (!std::isnan (z));
}


util::vector
operator* (double a, const util::vector &b)
    { return b * a; }


std::ostream&
operator<< (std::ostream &os, const util::vector &v) {
    os << "vec(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}


const json::node&
operator>> (const json::node &node, util::vector &v) {
    v.x = node[0].to_number ();
    v.y = node[1].to_number ();
    v.z = node[2].to_number ();

    return node;
}
