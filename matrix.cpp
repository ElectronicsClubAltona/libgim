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

#include "point.hpp"
#include "debug.hpp"

#include <cstring>
#include <cmath>

using namespace util;


//-----------------------------------------------------------------------------
template <typename T>
matrix<T>
matrix<T>::transposed (void) const
{
    matrix<T> m;
    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 4; ++j)
            m.values[i][j] = values[j][i];
    return m;
}


//-----------------------------------------------------------------------------
template <typename T>
matrix<T>&
matrix<T>::transpose (void)
{
    for (size_t i = 0; i < 4; ++i)
        for (size_t j = i + 1; j < 4; ++j)
            std::swap (values[i][j], values[j][i]);

    return *this;
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
    CHECK (is_affine ());

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

    for (unsigned row = 0; row < 4; ++row) {
        for (unsigned col = 0; col < 4; ++col) {
            m.values[row][col] = T {0};

            for (unsigned inner = 0; inner < 4; ++inner)
                m.values[row][col] += values[row][inner] * rhs.values[inner][col];
        }
    }

    return m;
}


//-----------------------------------------------------------------------------
template <typename T>
matrix<T>&
matrix<T>::operator*=(const matrix<T> &rhs) {
    return *this = *this * rhs;
}


//-----------------------------------------------------------------------------
template <typename T>
vector<4,T>
matrix<T>::operator* (const vector<4,T> &rhs) const {
    return vector<4,T> {
        values[0][0] * rhs.x + values[0][1] * rhs.y + values[0][2] * rhs.z + values[0][3] * rhs.w,
        values[1][0] * rhs.x + values[1][1] * rhs.y + values[1][2] * rhs.z + values[1][3] * rhs.w,
        values[2][0] * rhs.x + values[2][1] * rhs.y + values[2][2] * rhs.z + values[2][3] * rhs.w,
        values[3][0] * rhs.x + values[3][1] * rhs.y + values[3][2] * rhs.z + values[3][3] * rhs.w
    };
}


//-----------------------------------------------------------------------------
template <typename T>
point<4,T>
matrix<T>::operator* (const point<4,T> &rhs) const
{
    return point<4,T> {
        values[0][0] * rhs.x + values[0][1] * rhs.y + values[0][2] * rhs.z + values[0][3] * rhs.w,
        values[1][0] * rhs.x + values[1][1] * rhs.y + values[1][2] * rhs.z + values[1][3] * rhs.w,
        values[2][0] * rhs.x + values[2][1] * rhs.y + values[2][2] * rhs.z + values[2][3] * rhs.w,
        values[3][0] * rhs.x + values[3][1] * rhs.y + values[3][2] * rhs.z + values[3][3] * rhs.w
    };
}


//-----------------------------------------------------------------------------
template <typename T>
matrix<T>
matrix<T>::operator* (T f) const
{
    matrix<T> out;

    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 4; ++j)
            out.values[i][j] = values[i][j] * f;

    return out;
}


//-----------------------------------------------------------------------------
template <typename T>
matrix<T>&
matrix<T>::operator*= (T f){
    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 4; ++j)
            values[i][j] *= f;

    return *this;
}


//-----------------------------------------------------------------------------
template <typename T>
matrix<T>
matrix<T>::operator/ (T s) const {
    matrix<T> m;

    for (size_t r = 0; r < m.rows; ++r)
        for (size_t c = 0; c < m.cols; ++c)
            m.values[r][c] = values[r][c] / s;

    return m;
}


//-----------------------------------------------------------------------------
template <typename T>
matrix<T>&
matrix<T>::operator/= (T s) {
    for (size_t r = 0; r < rows; ++r)
        for (size_t c = 0; c < cols; ++c)
            values[r][c] /= s;

    return *this;
}


//-----------------------------------------------------------------------------
template <typename T>
bool
matrix<T>::operator== (const matrix<T> &rhs) const {
    for (size_t r = 0; r < rows; ++r)
        for (size_t c = 0; c < cols; ++c)
            if (!almost_equal (rhs.values[r][c], values[r][c]))
                return false;
    return true;
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
matrix<T>
matrix<T>::ortho (T left, T right,
                  T bottom, T top,
                  T near, T far)
{
    CHECK_GT (far, near);

    T tx = - (right + left) / (right - left);
    T ty = - (top + bottom) / (top - bottom);
    T tz = - (far + near)   / (far - near);

    T rl = 2 / (right - left);
    T tb = 2 / (top - bottom);
    T fn = 2 / (far - near);

    return { {
        { rl,  0,  0, tx },
        {  0, tb,  0, ty },
        {  0,  0, fn, tz },
        {  0,  0,  0,  1 },
    } };
}


//-----------------------------------------------------------------------------
template <typename T>
matrix<T>
matrix<T>::ortho2D (T left, T right,
                    T bottom, T top)
{
    return ortho (left, right, bottom, top, -1, 1);
}


//-----------------------------------------------------------------------------
template <typename T>
matrix<T>
matrix<T>::perspective (T fov, T aspect, T near, T far)
{
    T f = std::tan (fov / 2);

    T tx = 1 / (f * aspect);
    T ty = 1 / f;
    T z1 =     (far + near) / (near - far);
    T z2 = (2 * far * near) / (near - far);

    return { {
        { tx,  0,  0,  0 },
        {  0, ty,  0,  0 },
        {  0,  0, z1, z2 },
        {  0,  0, -1,  0 }
    } };
}


//-----------------------------------------------------------------------------
// Emulates gluLookAt
template <typename T>
matrix<T>
matrix<T>::look_at (util::point<3,T> eye,
                    util::point<3,T> centre,
                    util::vector<3,T> up)
{
    const auto f = eye.to (centre).normalise ();
    const auto s = cross (f, up).normalise ();
    const auto u = cross (s, f);

    return { {
        {  s.x,  s.y,  s.z, -dot (s, eye) },
        {  u.x,  u.y,  u.z, -dot (u, eye) },
        { -f.x, -f.y, -f.z,  dot (f, eye) },
        {    0,    0,    0,  1 },
    } };
}


//-----------------------------------------------------------------------------
template <typename T>
matrix<T>
matrix<T>::translate (util::vector<3,T> v)
{
    return { {
        { 1.f, 0.f, 0.f, v.x },
        { 0.f, 1.f, 0.f, v.y },
        { 0.f, 0.f, 1.f, v.z },
        { 0.f, 0.f, 0.f, 1.f },
    } };
}


//-----------------------------------------------------------------------------
template <typename T>
matrix<T>
matrix<T>::scale (T mag)
{
    return scale (vector<3,T> (mag));
}

//-----------------------------------------------------------------------------
template <typename T>
matrix<T>
matrix<T>::scale (util::vector<3,T> v)
{
    return { {
        { v.x, 0.f, 0.f, 0.f },
        { 0.f, v.y, 0.f, 0.f },
        { 0.f, 0.f, v.z, 0.f },
        { 0.f, 0.f, 0.f, 1.f }
    } };
}


//-----------------------------------------------------------------------------
template <typename T>
matrix<T>
matrix<T>::rotate (T angle, util::vector<3,T> about)
{
    about.normalise ();

    T c = std::cos (angle);
    T s = std::sin (angle);
    T x = about.x,
      y = about.y,
      z = about.z;

    return { {
        { x * x * (1 - c) + c,
          x * y * (1 - c) - z * s,
          x * z * (1 - c) + y * s,
          0
        },

        { y * x * (1 - c) + z * s,
          y * y * (1 - c) + c,
          y * z * (1 - c) - x * s,
          0
        },

        { z * x * (1 - c) - y * s,
          z * y * (1 - c) + x * s,
          z * z * (1 - c) + c,
          0
        },

        { 0, 0, 0, 1 }
    } };
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
namespace util {
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
}

template std::ostream& util::operator<< (std::ostream&, const matrix<float>&);
template std::ostream& util::operator<< (std::ostream&, const matrix<double>&);
