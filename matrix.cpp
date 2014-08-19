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
 * Copyright 2011-2014 Danny Robson <danny@nerdcruft.net>
 */

#include "matrix.hpp"

#include "debug.hpp"

#include <cstring>

using namespace util;


//-----------------------------------------------------------------------------
template <typename T>
void
matrix<T>::scale (T x, T y, T z) {
    CHECK_HARD (is_affine ());
    values[0][0] *= x;
    values[1][1] *= y;
    values[2][2] *= z;
}


//-----------------------------------------------------------------------------
template <typename T>
void
matrix<T>::translate (T x, T y, T z) {
    CHECK_HARD (is_affine ());
    values[0][3] += x;
    values[1][3] += y;
    values[2][3] += z;
}


//-----------------------------------------------------------------------------
template <typename T>
matrix<T>
matrix<T>::inverse (void) const {
    matrix<T> m;

    T d = det ();
    if (almost_zero (d))
        throw std::runtime_error ("non-singular matrix");
    auto v = values;

    m.values[0][0] = v[1][2] * v[2][3] * v[3][1] -
                     v[1][3] * v[2][2] * v[3][1] +
                     v[1][3] * v[2][1] * v[3][2] -
                     v[1][1] * v[2][3] * v[3][2] -
                     v[1][2] * v[2][1] * v[3][3] +
                     v[1][1] * v[2][2] * v[3][3];

    m.values[0][1] = v[0][3] * v[2][2] * v[3][1] -
                     v[0][2] * v[2][3] * v[3][1] -
                     v[0][3] * v[2][1] * v[3][2] +
                     v[0][1] * v[2][3] * v[3][2] +
                     v[0][2] * v[2][1] * v[3][3] -
                     v[0][1] * v[2][2] * v[3][3];

    m.values[0][2] = v[0][2] * v[1][3] * v[3][1] -
                     v[0][3] * v[1][2] * v[3][1] +
                     v[0][3] * v[1][1] * v[3][2] -
                     v[0][1] * v[1][3] * v[3][2] -
                     v[0][2] * v[1][1] * v[3][3] +
                     v[0][1] * v[1][2] * v[3][3];

    m.values[0][3] = v[0][3] * v[1][2] * v[2][1] -
                     v[0][2] * v[1][3] * v[2][1] -
                     v[0][3] * v[1][1] * v[2][2] +
                     v[0][1] * v[1][3] * v[2][2] +
                     v[0][2] * v[1][1] * v[2][3] -
                     v[0][1] * v[1][2] * v[2][3];

    m.values[1][0] = v[1][3] * v[2][2] * v[3][0] -
                     v[1][2] * v[2][3] * v[3][0] -
                     v[1][3] * v[2][0] * v[3][2] +
                     v[1][0] * v[2][3] * v[3][2] +
                     v[1][2] * v[2][0] * v[3][3] -
                     v[1][0] * v[2][2] * v[3][3];

    m.values[1][1] = v[0][2] * v[2][3] * v[3][0] -
                     v[0][3] * v[2][2] * v[3][0] +
                     v[0][3] * v[2][0] * v[3][2] -
                     v[0][0] * v[2][3] * v[3][2] -
                     v[0][2] * v[2][0] * v[3][3] +
                     v[0][0] * v[2][2] * v[3][3];

    m.values[1][2] = v[0][3] * v[1][2] * v[3][0] -
                     v[0][2] * v[1][3] * v[3][0] -
                     v[0][3] * v[1][0] * v[3][2] +
                     v[0][0] * v[1][3] * v[3][2] +
                     v[0][2] * v[1][0] * v[3][3] -
                     v[0][0] * v[1][2] * v[3][3];

    m.values[1][3] = v[0][2] * v[1][3] * v[2][0] -
                     v[0][3] * v[1][2] * v[2][0] +
                     v[0][3] * v[1][0] * v[2][2] -
                     v[0][0] * v[1][3] * v[2][2] -
                     v[0][2] * v[1][0] * v[2][3] +
                     v[0][0] * v[1][2] * v[2][3];

    m.values[2][0] = v[1][1] * v[2][3] * v[3][0] -
                     v[1][3] * v[2][1] * v[3][0] +
                     v[1][3] * v[2][0] * v[3][1] -
                     v[1][0] * v[2][3] * v[3][1] -
                     v[1][1] * v[2][0] * v[3][3] +
                     v[1][0] * v[2][1] * v[3][3];

    m.values[2][1] = v[0][3] * v[2][1] * v[3][0] -
                     v[0][1] * v[2][3] * v[3][0] -
                     v[0][3] * v[2][0] * v[3][1] +
                     v[0][0] * v[2][3] * v[3][1] +
                     v[0][1] * v[2][0] * v[3][3] -
                     v[0][0] * v[2][1] * v[3][3];

    m.values[2][2] = v[0][1] * v[1][3] * v[3][0] -
                     v[0][3] * v[1][1] * v[3][0] +
                     v[0][3] * v[1][0] * v[3][1] -
                     v[0][0] * v[1][3] * v[3][1] -
                     v[0][1] * v[1][0] * v[3][3] +
                     v[0][0] * v[1][1] * v[3][3];

    m.values[2][3] = v[0][3] * v[1][1] * v[2][0] -
                     v[0][1] * v[1][3] * v[2][0] -
                     v[0][3] * v[1][0] * v[2][1] +
                     v[0][0] * v[1][3] * v[2][1] +
                     v[0][1] * v[1][0] * v[2][3] -
                     v[0][0] * v[1][1] * v[2][3];

    m.values[3][0] = v[1][2] * v[2][1] * v[3][0] -
                     v[1][1] * v[2][2] * v[3][0] -
                     v[1][2] * v[2][0] * v[3][1] +
                     v[1][0] * v[2][2] * v[3][1] +
                     v[1][1] * v[2][0] * v[3][2] -
                     v[1][0] * v[2][1] * v[3][2];

    m.values[3][1] = v[0][1] * v[2][2] * v[3][0] -
                     v[0][2] * v[2][1] * v[3][0] +
                     v[0][2] * v[2][0] * v[3][1] -
                     v[0][0] * v[2][2] * v[3][1] -
                     v[0][1] * v[2][0] * v[3][2] +
                     v[0][0] * v[2][1] * v[3][2];

    m.values[3][2] = v[0][2] * v[1][1] * v[3][0] -
                     v[0][1] * v[1][2] * v[3][0] -
                     v[0][2] * v[1][0] * v[3][1] +
                     v[0][0] * v[1][2] * v[3][1] +
                     v[0][1] * v[1][0] * v[3][2] -
                     v[0][0] * v[1][1] * v[3][2];

    m.values[3][3] = v[0][1] * v[1][2] * v[2][0] -
                     v[0][2] * v[1][1] * v[2][0] +
                     v[0][2] * v[1][0] * v[2][1] -
                     v[0][0] * v[1][2] * v[2][1] -
                     v[0][1] * v[1][0] * v[2][2] +
                     v[0][0] * v[1][1] * v[2][2];

    m /= d;
    return m;
}


//-----------------------------------------------------------------------------
template <typename T>
matrix<T>&
matrix<T>::invert (void) {
    auto m = *this;
    m.invert ();
    *this = m;

    return *this;
}


//-----------------------------------------------------------------------------
template <typename T>
matrix<T>
matrix<T>::inverse_affine (void) const {
    return matrix<T>(*this).invert_affine ();
}


//-----------------------------------------------------------------------------
template <typename T>
matrix<T>&
matrix<T>::invert_affine (void) {
    CHECK_HARD (is_affine ());

    // inv ([ M b ]  == [ inv(M) -inv(M).b ]
    //      [ 0 1 ])    [     0       1    ]

    // Invert the 3x3 M
    T A = (values[1][1] * values[2][2] - values[1][2] * values[2][1]);
    T B = (values[1][2] * values[2][0] - values[1][0] * values[2][2]);
    T C = (values[1][0] * values[2][1] - values[1][1] * values[2][0]);

    T D = (values[0][2] * values[2][1] - values[0][1] * values[2][2]);
    T E = (values[0][0] * values[2][2] - values[0][2] * values[2][0]);
    T F = (values[2][0] * values[0][1] - values[0][0] * values[2][1]);

    T G = (values[0][1] * values[1][2] - values[0][2] * values[1][1]);
    T H = (values[0][2] * values[1][0] - values[0][0] * values[1][2]);
    T K = (values[0][0] * values[1][1] - values[0][1] * values[1][0]);

    T d = values[0][0] * A + values[0][1] * B + values[0][2] * C;
    CHECK_NEQ (d, 0.0);

    values[0][0] = A / d;
    values[0][1] = D / d;
    values[0][2] = G / d;
    values[1][0] = B / d;
    values[1][1] = E / d;
    values[1][2] = H / d;
    values[2][0] = C / d;
    values[2][1] = F / d;
    values[2][2] = K / d;

    // Multiply the b
    T b0 = - values[0][0] * values[0][3] - values[0][1] * values[1][3] - values[0][2] * values[2][3];
    T b1 = - values[1][0] * values[0][3] - values[1][1] * values[1][3] - values[1][2] * values[2][3];
    T b2 = - values[2][0] * values[0][3] - values[2][1] * values[1][3] - values[2][2] * values[2][3];

    values[0][3] = b0;
    values[1][3] = b1;
    values[2][3] = b2;

    return *this;
}


//-----------------------------------------------------------------------------
template <typename T>
T
matrix<T>::det (void) const {
    return values[0][3] * values[1][2] * values[2][1] * values[3][0] -
           values[0][2] * values[1][3] * values[2][1] * values[3][0] -
           values[0][3] * values[1][1] * values[2][2] * values[3][0] +
           values[0][1] * values[1][3] * values[2][2] * values[3][0] +
           values[0][2] * values[1][1] * values[2][3] * values[3][0] -
           values[0][1] * values[1][2] * values[2][3] * values[3][0] -

           values[0][3] * values[1][2] * values[2][0] * values[3][1] +
           values[0][2] * values[1][3] * values[2][0] * values[3][1] +
           values[0][3] * values[1][0] * values[2][2] * values[3][1] -
           values[0][0] * values[1][3] * values[2][2] * values[3][1] -
           values[0][2] * values[1][0] * values[2][3] * values[3][1] +
           values[0][0] * values[1][2] * values[2][3] * values[3][1] +

           values[0][3] * values[1][1] * values[2][0] * values[3][2] -
           values[0][1] * values[1][3] * values[2][0] * values[3][2] -
           values[0][3] * values[1][0] * values[2][1] * values[3][2] +
           values[0][0] * values[1][3] * values[2][1] * values[3][2] +
           values[0][1] * values[1][0] * values[2][3] * values[3][2] -
           values[0][0] * values[1][1] * values[2][3] * values[3][2] -

           values[0][2] * values[1][1] * values[2][0] * values[3][3] +
           values[0][1] * values[1][2] * values[2][0] * values[3][3] +
           values[0][2] * values[1][0] * values[2][1] * values[3][3] -
           values[0][0] * values[1][2] * values[2][1] * values[3][3] -
           values[0][1] * values[1][0] * values[2][2] * values[3][3] +
           values[0][0] * values[1][1] * values[2][2] * values[3][3];
}


//-----------------------------------------------------------------------------
template <typename T>
matrix<T>
matrix<T>::operator* (const matrix<T> &rhs) const {
    matrix<T> m;
    memset (m.values, 0, sizeof (m.values));

    for (unsigned i = 0; i < 4; ++i)
        for (unsigned j = 0; j < 4; ++j)
            for (unsigned k = 0; k < 4; ++k)
                m.values[i][j] += values[i][k] * rhs.values[k][j];

    return m;
}


//-----------------------------------------------------------------------------
template <typename T>
util::point<3>
matrix<T>::to_local (const util::point<3> &p) const {
    CHECK_SOFT (is_affine ());

    return { p.x * values[0][0] +
             p.y * values[0][1] +
             p.z * values[0][2] + values[0][3],
             p.x * values[1][0] +
             p.y * values[1][1] +
             p.z * values[1][2] + values[1][3],
             p.x * values[2][0] +
             p.y * values[2][1] +
             p.z * values[2][2] + values[2][3] };
}


//-----------------------------------------------------------------------------
template <typename T>
util::point<3>
matrix<T>::to_global (const util::point<3> &p) const {
    return inverse ().to_local (p);
}


//-----------------------------------------------------------------------------
template <typename T>
bool
matrix<T>::is_affine (void) const {
    return exactly_equal (values[3][0], T {0}) &&
           exactly_equal (values[3][1], T {0}) &&
           exactly_equal (values[3][2], T {0}) &&
           exactly_equal (values[3][3], T {1});
}


//-----------------------------------------------------------------------------
template <typename T>
const matrix<T>
matrix<T>::IDENTITY = { { { 1, 0, 0, 0 },
                          { 0, 1, 0, 0 },
                          { 0, 0, 1, 0 },
                          { 0, 0, 0, 1 } } };


template <typename T>
const matrix<T>
matrix<T>::ZEROES = { { { 0, 0, 0, 0 },
                        { 0, 0, 0, 0 },
                        { 0, 0, 0, 0 },
                        { 0, 0, 0, 0 } } };


//-----------------------------------------------------------------------------
namespace util {
    template struct matrix<float>;
    template struct matrix<double>;
}

//-----------------------------------------------------------------------------
template <typename T>
std::ostream&
operator<< (std::ostream &os, const matrix<T> &m) {
    os << "{ {" << m.values[0][0] << ", "
                << m.values[0][1] << ", "
                << m.values[0][2] << ", "
                << m.values[0][3] << "}, "
       <<   "{" << m.values[1][0] << ", "
                << m.values[1][1] << ", "
                << m.values[1][2] << ", "
                << m.values[1][3] << "}, "
       <<   "{" << m.values[2][0] << ", "
                << m.values[2][1] << ", "
                << m.values[2][2] << ", "
                << m.values[2][3] << "}, "
       <<   "{" << m.values[3][0] << ", "
                << m.values[3][1] << ", "
                << m.values[3][2] << ", "
                << m.values[3][3] << "} }";

    return os;
}

