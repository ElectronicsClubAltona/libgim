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
    // GLM's implementation of 4x4 matrix inversion. Should allow use of
    // vector instructions.
    const auto &m = values;

    T Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
    T Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
    T Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

    T Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
    T Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
    T Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

    T Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
    T Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
    T Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

    T Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
    T Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
    T Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

    T Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
    T Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
    T Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

    T Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
    T Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
    T Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];


    vector<4,T> Fac0(Coef00, Coef00, Coef02, Coef03);
    vector<4,T> Fac1(Coef04, Coef04, Coef06, Coef07);
    vector<4,T> Fac2(Coef08, Coef08, Coef10, Coef11);
    vector<4,T> Fac3(Coef12, Coef12, Coef14, Coef15);
    vector<4,T> Fac4(Coef16, Coef16, Coef18, Coef19);
    vector<4,T> Fac5(Coef20, Coef20, Coef22, Coef23);

    vector<4,T> Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
    vector<4,T> Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
    vector<4,T> Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
    vector<4,T> Vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

    vector<4,T> Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
    vector<4,T> Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
    vector<4,T> Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
    vector<4,T> Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

    vector<4,T> SignA(+1, -1, +1, -1);
    vector<4,T> SignB(-1, +1, -1, +1);
    //matrix<T>   Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);
    matrix<T>   Inverse = { { { Inv0.x * SignA.x, Inv0.y * SignA.y, Inv0.z * SignA.z, Inv0.w * SignA.w },
                              { Inv1.x * SignB.x, Inv1.y * SignB.y, Inv1.z * SignB.z, Inv1.w * SignB.w },
                              { Inv2.x * SignA.x, Inv2.y * SignA.y, Inv2.z * SignA.z, Inv2.w * SignA.w },
                              { Inv3.x * SignB.x, Inv3.y * SignB.y, Inv3.z * SignB.z, Inv3.w * SignB.w } } };

    vector<4,T> Row0(Inverse.values[0][0], Inverse.values[1][0], Inverse.values[2][0], Inverse.values[3][0]);

    vector<4,T> Dot0(
        m[0][0] * Row0.x,
        m[0][1] * Row0.y,
        m[0][2] * Row0.z,
        m[0][3] * Row0.w
    );
    T Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

    T OneOverDeterminant = static_cast<T>(1) / Dot1;

    return Inverse * OneOverDeterminant;

}


//-----------------------------------------------------------------------------
template <typename T>
matrix<T>&
matrix<T>::invert (void) {
    return *this = inverse ();
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


///////////////////////////////////////////////////////////////////////////////
template <typename T>
vector<3,T>
matrix<T>::operator* (vector<3,T> v) const
{
    return (
        *this * v.template homog<4> ()
    ).template redim<3> ();
}


//-----------------------------------------------------------------------------
template <typename T>
point<3,T>
matrix<T>::operator* (point<3,T> p) const
{
    return (*this * p.template homog<4> ()).template redim<3> ();
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
    const auto f = (centre - eye).normalise ();
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
matrix<T>::translate (util::vector<2,T> v)
{
    return translate ({v.x, v.y, 0});
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
