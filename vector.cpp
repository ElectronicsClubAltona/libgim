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
template <size_t S, typename T>
util::vector<S,T>
util::vector<S,T>::operator* (T rhs) const {
    util::vector<S,T> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] * rhs;

    return out;
}


template <size_t S, typename T>
util::vector<S,T>&
util::vector<S,T>::operator*= (T rhs) {
    for (auto &i: this->data)
        i *= rhs;

    return *this;
}


template <size_t S, typename T>
util::vector<S,T>
util::vector<S,T>::operator* (const vector<S,T> &rhs) const {
    util::vector<S,T> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] * rhs.data[i];

    return out;
}


template <size_t S, typename T>
util::vector<S,T>&
util::vector<S,T>::operator*= (const vector<S,T> &rhs) {
    for (size_t i = 0; i < S; ++i)
        this->data[i] *= rhs.data[i];

    return *this;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::vector<S,T>
util::vector<S,T>::operator/ (T rhs) const {
    util::vector<S,T> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] / rhs;
    return out;
}


template <size_t S, typename T>
util::vector<S,T>&
util::vector<S,T>::operator/= (T rhs) {
    for (size_t i = 0; i < S; ++i)
        this->data[i] /= rhs;
    return *this;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::vector<S,T>
util::vector<S,T>::operator+ (const util::vector<S,T> &rhs) const {
    util::vector<S,T> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] + rhs.data[i];
    return out;
}


template <size_t S, typename T>
util::vector<S,T>
util::vector<S,T>::operator+ (T rhs) const {
    util::vector<S,T> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] + rhs;
    return out;
}


template <size_t S, typename T>
util::vector<S,T>&
util::vector<S,T>::operator+= (const util::vector<S,T> &rhs) {
    for (size_t i = 0; i < S; ++i)
        this->data[i] += rhs.data[i];

    return *this;
}


template <size_t S, typename T>
util::vector<S,T>&
util::vector<S,T>::operator+= (T rhs) {
    for (size_t i = 0; i < S; ++i)
        this->data[i] += rhs;
    return *this;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::vector<S,T>
util::vector<S,T>::operator- (void) const {
    util::vector<S,T> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = -this->data[i];
    return out;
}


template <size_t S, typename T>
util::vector<S,T>
util::vector<S,T>::operator- (const util::vector<S,T> &rhs) const {
    util::vector<S,T> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] - rhs.data[i];
    return out;
}


template <size_t S, typename T>
util::vector<S,T>&
util::vector<S,T>::operator-= (const util::vector<S,T> &rhs) {
    for (size_t i = 0; i < S; ++i)
        this->data[i] -= rhs.data[i];

    return *this;
}


template <size_t S, typename T>
util::vector<S,T>
util::vector<S,T>::operator- (T rhs) const {
    util::vector<S,T> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] - rhs;
    return out;
}


template <size_t S, typename T>
util::vector<S,T>&
util::vector<S,T>::operator-= (T rhs) {
    for (size_t i = 0; i < S; ++i)
        this->data[i] -= rhs;

    return *this;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::vector<S,T>&
util::vector<S,T>::operator= (const util::vector<S,T> &rhs) {
    std::copy (begin (rhs.data), end (rhs.data), begin (this->data));

    return *this;
}


template <size_t S, typename T>
bool
util::vector<S,T>::operator== (const util::vector<S,T> &rhs) const {
    for (size_t i = 0; i < S; ++i)
        if (!almost_equal (this->data[i], rhs.data[i]))
            return false;

    return true;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
T
util::vector<S,T>::magnitude (void) const {
    // TODO: this should not truncate for integral types
    return static_cast<T> (std::sqrt (magnitude2 ()));
}


template <size_t S, typename T>
T
util::vector<S,T>::magnitude2 (void) const {
    T total { 0 };
    for (size_t i = 0; i < S; ++i)
        total += pow2 (this->data[i]);
    return total;
}


template <size_t S, typename T>
util::vector<S,T>&
util::vector<S,T>::normalise (void) {
    T mag = magnitude ();

    for (size_t i = 0; i < S; ++i)
        this->data[i] /= mag;

    return *this;
}


template <size_t S, typename T>
util::vector<S,T>
util::vector<S,T>::normalised (void) const {
    T mag = magnitude ();
    util::vector<S,T> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] / mag;

    return out;
}

//-----------------------------------------------------------------------------
template <typename T>
util::vector<2,T>
util::polar_to_cartesian (const util::vector<2,T> &v) {
    return util::vector<2,T> {
        v.r * std::cos (v.t),
        v.r * std::sin (v.t)
    };
}

//-----------------------------------------------------------------------------
template <size_t S, typename T>
T
util::vector<S,T>::dot (const util::vector<S,T> &rhs) const {
    T total { 0 };
    for (size_t i = 0; i < S; ++i)
        total += this->data[i] * rhs.data[i];
    return total;
}


template <typename T>
util::vector<3,T>
util::cross (const util::vector<3,T> &a,
             const util::vector<3,T> &b)
{
    return util::vector<3,T> {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

template util::vector3f util::cross(const util::vector3f&, const util::vector3f&);
template util::vector3d util::cross(const util::vector3d&, const util::vector3d&);


//-----------------------------------------------------------------------------
template <typename T>
util::vector<3,T>
util::spherical_to_cartesian (const util::vector<3,T> &s) {
    return util::vector<3,T> {
        s.x * sin (s.y) * cos (s.z),
        s.x * sin (s.y) * sin (s.z),
        s.x * cos (s.y),
    };
}


template <typename T>
util::vector<3,T>
util::cartesian_to_spherical (const util::vector<3,T> &c) {
    T mag = c.magnitude ();

    return util::vector<3,T> {
        mag,
        acos  (c.z / mag),
        atan2 (c.y, c.x)
    };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
bool
util::vector<S,T>::is_zero (void) const {
    return std::all_of (begin (this->data),
                        end   (this->data),
                        [] (T i) { return almost_zero (i); });
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
const util::vector<S,T>
util::vector<S,T>::ZERO (T{0});


//-----------------------------------------------------------------------------
template <size_t S, typename T>
void
util::vector<S,T>::sanity (void) const {
    CHECK_SOFT (std::all_of (begin (this->data),
                             end   (this->data),
                             [] (T i) { return !std::isnan (i); }));
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::vector<S,T>
util::operator* (T a, const util::vector<S,T> &b)
    { return b * a; }


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::vector<S,T>
util::operator+ (T a, const util::vector<S,T> &b)
    { return b + a; }


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::vector<S,T>
util::operator- (T a, const util::vector<S,T> &b)
    { return a + (-b); }


//-----------------------------------------------------------------------------
template <size_t S, typename T>
std::ostream&
util::operator<< (std::ostream &os, const util::vector<S,T> &v) {
    os << "vec" << S << "(" << v.data[0];
    for (size_t i = 1; i < S; ++i)
        os << ", " << v.data[i];
    os << ")";
    return os;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
const json::node&
util::operator>> (const json::node &node, util::vector<S,T> &v) {
    const json::array &array = node.as_array ();
    if (array.size () != S)
        throw std::runtime_error ("Invalid dimensionality for json-to-vector");

    // XXX: This used to be a std::transform but gcc 4.9.0 hit an internal
    // compiler error at this point in release mode, so we dumb it down a
    // little.
    for (size_t i = 0; i < array.size (); ++i)
        v.data[i] = static_cast<T> (array[i].as_number ().native ());

    return node;
}


//-----------------------------------------------------------------------------

#define INSTANTIATE_S_T(S,T)                                                        \
template struct util::vector<S,T>;                                                  \
template util::vector<S,T> util::operator* (T, const util::vector<S,T>&);           \
template util::vector<S,T> util::operator+ (T, const util::vector<S,T>&);           \
template util::vector<S,T> util::operator- (T, const util::vector<S,T>&);           \
template std::ostream& util::operator<< (std::ostream&, const util::vector<S,T> &v);\
template const json::node& util::operator>> (const json::node&, util::vector<S,T>&);


#define INSTANTIATE(T)  \
INSTANTIATE_S_T(1,T)    \
INSTANTIATE_S_T(2,T)    \
INSTANTIATE_S_T(3,T)    \
INSTANTIATE_S_T(4,T)


INSTANTIATE(uint32_t)
INSTANTIATE(int32_t)
INSTANTIATE(uint64_t)
INSTANTIATE(int64_t)
INSTANTIATE(float)
INSTANTIATE(double)

//-----------------------------------------------------------------------------
namespace util {
    template <> vector<1,float> random (void) { util::vector<1,float> out; randomise (out.data); return out; }
    template <> vector<2,float> random (void) { util::vector<2,float> out; randomise (out.data); return out; }
    template <> vector<3,float> random (void) { util::vector<3,float> out; randomise (out.data); return out; }
    template <> vector<4,float> random (void) { util::vector<4,float> out; randomise (out.data); return out; }

    template <> vector<1,double> random (void) { util::vector<1,double> out; randomise (out.data); return out; }
    template <> vector<2,double> random (void) { util::vector<2,double> out; randomise (out.data); return out; }
    template <> vector<3,double> random (void) { util::vector<3,double> out; randomise (out.data); return out; }
    template <> vector<4,double> random (void) { util::vector<4,double> out; randomise (out.data); return out; }
}

