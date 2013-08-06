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

#include "vector.hpp"

#include "debug.hpp"
#include "maths.hpp"
#include "random.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>

#if defined(COMPILER_GCC)
#pragma GCC optimize("-O3")
#endif

//-----------------------------------------------------------------------------
using namespace util;
using std::begin;
using std::end;


//-----------------------------------------------------------------------------
template <size_t S>
util::vector<S>::vector ()
{ ; }


//-----------------------------------------------------------------------------
template <size_t S>
util::vector<S>
util::vector<S>::operator* (double rhs) const {
    util::vector<S> out;
    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] * rhs;
    return out;
}


template <size_t S>
util::vector<S>&
util::vector<S>::operator*= (double rhs) {
    for (double &i: this->data)
        i *= rhs;

    return *this;
}


template <size_t S>
util::vector<S>
util::vector<S>::operator* (const util::vector<S> &rhs) const {
    util::vector<S> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] * rhs.data[i];
    return out;
}


template <size_t S>
util::vector<S>&
util::vector<S>::operator*= (const util::vector<S> &rhs) {
    for (size_t i = 0; i < S; ++i)
        this->data[i] *= rhs.data[i];

    return *this;
}


//-----------------------------------------------------------------------------
template <size_t S>
util::vector<S>
util::vector<S>::operator/ (double rhs) const {
    util::vector<S> out;
    
    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] / rhs;
    return out;
}


template <size_t S>
util::vector<S>&
util::vector<S>::operator/= (double rhs) {
    for (size_t i = 0; i < S; ++i)
        this->data[i] /= rhs;
    return *this;
}


//-----------------------------------------------------------------------------
template <size_t S>
util::vector<S>
util::vector<S>::operator+ (const util::vector<S> &rhs) const {
    util::vector<S> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] + rhs.data[i];
    return out;
}


template <size_t S>
util::vector<S>
util::vector<S>::operator+ (double rhs) const {
    util::vector<S> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] + rhs;
    return out;
}


template <size_t S>
util::vector<S>&
util::vector<S>::operator+= (const util::vector<S> &rhs) {
    for (size_t i = 0; i < S; ++i)
        this->data[i] += rhs.data[i];

    return *this;
}


template <size_t S>
util::vector<S>&
util::vector<S>::operator+= (double rhs) {
    for (size_t i = 0; i < S; ++i)
        this->data[i] += rhs;
    return *this;
}


//-----------------------------------------------------------------------------
template <size_t S>
util::vector<S>
util::vector<S>::operator- (void) const {
    util::vector<S> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = -this->data[i];
    return out;
}


template <size_t S>
util::vector<S>
util::vector<S>::operator- (const util::vector<S> &rhs) const {
    util::vector<S> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] - rhs.data[i];
    return out;
}


template <size_t S>
util::vector<S>&
util::vector<S>::operator-= (const util::vector<S> &rhs) {
    for (size_t i = 0; i < S; ++i)
        this->data[i] -= rhs.data[i];

    return *this;
}


template <size_t S>
util::vector<S>
util::vector<S>::operator- (double rhs) const {
    util::vector<S> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] - rhs;
    return out;
}


template <size_t S>
util::vector<S>&
util::vector<S>::operator-= (double rhs) {
    for (size_t i = 0; i < S; ++i)
        this->data[i] -= rhs;

    return *this;
}


//-----------------------------------------------------------------------------
template <size_t S>
util::vector<S>&
util::vector<S>::operator= (const util::vector<S> &rhs) {
    std::copy (begin (rhs.data), end (rhs.data), begin (this->data));

    return *this;
}


template <size_t S>
bool
util::vector<S>::operator== (const util::vector<S> &rhs) const {
    for (size_t i = 0; i < S; ++i)
        if (!almost_equal (this->data[i], rhs.data[i]))
            return false;

    return true;
}


//-----------------------------------------------------------------------------
template <size_t S>
double
util::vector<S>::magnitude (void) const {
    return sqrt (magnitude2 ());
}


template <size_t S>
double
util::vector<S>::magnitude2 (void) const {
    double total = 0.0;
    for (size_t i = 0; i < S; ++i)
        total += pow2 (this->data[i]);
    return total;
}


template <size_t S>
util::vector<S>&
util::vector<S>::normalise (void) {
    double mag = magnitude ();

    for (size_t i = 0; i < S; ++i)
        this->data[i] /= mag;

    return *this;
}


template <size_t S>
util::vector<S>
util::vector<S>::normalised (void) const {
    double mag = magnitude ();
    util::vector<S> out;
    
    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] / mag;

    return out;
}

//-----------------------------------------------------------------------------
util::vector<2>
util::polar_to_cartesian (const util::vector<2> &v) {
    return util::vector<2> {
        v.r * std::cos (v.t),
        v.r * std::sin (v.t)
    };
}

//-----------------------------------------------------------------------------
template <size_t S>
double
util::vector<S>::dot (const util::vector<S> &rhs) const {
    double total = 0.0;
    for (size_t i = 0; i < S; ++i)
        total += this->data[i] * rhs.data[i];
    return total;
}


util::vector<3>
util::cross (const util::vector<3> &a, const util::vector<3> &b) {
    return util::vector<3> {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}


//-----------------------------------------------------------------------------
util::vector<3>
util::spherical_to_cartesian (const util::vector<3> &s) {
    return util::vector<3> {
        s.x * sin (s.y) * cos (s.z),
        s.x * sin (s.y) * sin (s.z),
        s.x * cos (s.y),
    };
}


util::vector<3>
util::cartesian_to_spherical (const util::vector<3> &c) {
    double mag = c.magnitude ();

    return util::vector<3> {
        mag,
        acos  (c.z / mag),
        atan2 (c.y, c.x)
    };
}


//-----------------------------------------------------------------------------
template <size_t S>
bool
util::vector<S>::is_zero (void) const {
    return std::all_of (begin (this->data),
                        end   (this->data),
                        [] (double i) { return almost_zero (i); });
}


//-----------------------------------------------------------------------------
template <size_t S>
void
util::vector<S>::sanity (void) const {
    CHECK_SOFT (std::all_of (begin (this->data),
                             end   (this->data),
                             [] (double i) { return !std::isnan (i); }));
}


//-----------------------------------------------------------------------------
template <size_t S>
util::vector<S>
util::operator* (double a, const util::vector<S> &b)
    { return b * a; }


template util::vector<1> util::operator* (double, const util::vector<1>&);
template util::vector<2> util::operator* (double, const util::vector<2>&);
template util::vector<3> util::operator* (double, const util::vector<3>&);


template <size_t S>
util::vector<S>
util::operator+ (double a, const util::vector<S> &b)
    { return b + a; }


template util::vector<1> util::operator+ (double, const util::vector<1>&);
template util::vector<2> util::operator+ (double, const util::vector<2>&);
template util::vector<3> util::operator+ (double, const util::vector<3>&);


template <size_t S>
util::vector<S>
util::operator- (double a, const util::vector<S> &b)
    { return a + (-b); }


template util::vector<1> util::operator- (double, const util::vector<1>&);
template util::vector<2> util::operator- (double, const util::vector<2>&);
template util::vector<3> util::operator- (double, const util::vector<3>&);


//-----------------------------------------------------------------------------
template <size_t S>
std::ostream&
util::operator<< (std::ostream &os, const util::vector<S> &v) {
    os << "vec" << S << "(" << v.data[0];
    for (size_t i = 1; i < S; ++i)
        os << ", " << v.data[i];
    os << ")";
    return os;
}


template std::ostream& util::operator<< (std::ostream&, const util::vector<1> &v);
template std::ostream& util::operator<< (std::ostream&, const util::vector<2> &v);
template std::ostream& util::operator<< (std::ostream&, const util::vector<3> &v);


//-----------------------------------------------------------------------------
template <size_t S>
const json::node&
util::operator>> (const json::node &node, util::vector<S> &v) {
    const json::array &array = node.as_array ();
    if (array.size () != S) 
        throw std::runtime_error ("Invalid dimensionality for json-to-vector");

    std::transform (begin (array),
                    end   (array),
                    begin (v.data),
                    [] (const json::node &n) {
                        return n.as_number ().native ();
                    });
    return node;
}


template const json::node& util::operator>> (const json::node&, util::vector<1>&);
template const json::node& util::operator>> (const json::node&, util::vector<2>&);
template const json::node& util::operator>> (const json::node&, util::vector<3>&);


//-----------------------------------------------------------------------------
template struct util::vector<1>;
template struct util::vector<2>;
template struct util::vector<3>;


//-----------------------------------------------------------------------------
namespace util {
    template <> vector<1> random (void) { util::vector<1> out; randomise (out.data); return out; }
    template <> vector<2> random (void) { util::vector<2> out; randomise (out.data); return out; }
    template <> vector<3> random (void) { util::vector<3> out; randomise (out.data); return out; }
}

