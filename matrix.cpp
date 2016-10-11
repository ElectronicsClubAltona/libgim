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
 * Copyright 2011-2015 Danny Robson <danny@nerdcruft.net>
 */

#include "matrix.hpp"

#include "debug.hpp"
#include "iterator.hpp"
#include "point.hpp"

#include <cstring>
#include <cmath>

using namespace util;


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
matrix<S,T>
matrix<S,T>::transposed (void) const
{
    matrix<S,T> m;

    for (size_t i = 0; i < S; ++i)
        for (size_t j = 0; j < S; ++j)
            m.values[i][j] = values[j][i];

    return m;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
matrix<S,T>&
matrix<S,T>::transpose (void)
{
    for (size_t i = 0; i < S; ++i)
        for (size_t j = i + 1; j < S; ++j)
            std::swap (values[i][j], values[j][i]);

    return *this;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
matrix<S,T>&
matrix<S,T>::invert (void)
{
    return *this = inverse ();
}


//-----------------------------------------------------------------------------
//template <size_t S, typename T>
//matrix<S,T>&
//matrix<S,T>::invert_affine (void)
//{
//    CHECK (is_affine ());
//
//    // inv ([ M b ]  == [ inv(M) -inv(M).b ]
//    //      [ 0 1 ])    [     0       1    ]
//
//    // Invert the 3x3 M
//    T A = (values[1][1] * values[2][2] - values[1][2] * values[2][1]);
//    T B = (values[1][2] * values[2][0] - values[1][0] * values[2][2]);
//    T C = (values[1][0] * values[2][1] - values[1][1] * values[2][0]);
//
//    T D = (values[0][2] * values[2][1] - values[0][1] * values[2][2]);
//    T E = (values[0][0] * values[2][2] - values[0][2] * values[2][0]);
//    T F = (values[2][0] * values[0][1] - values[0][0] * values[2][1]);
//
//    T G = (values[0][1] * values[1][2] - values[0][2] * values[1][1]);
//    T H = (values[0][2] * values[1][0] - values[0][0] * values[1][2]);
//    T K = (values[0][0] * values[1][1] - values[0][1] * values[1][0]);
//
//    T d = values[0][0] * A + values[0][1] * B + values[0][2] * C;
//    CHECK_NEQ (d, 0.0);
//
//    values[0][0] = A / d;
//    values[0][1] = D / d;
//    values[0][2] = G / d;
//    values[1][0] = B / d;
//    values[1][1] = E / d;
//    values[1][2] = H / d;
//    values[2][0] = C / d;
//    values[2][1] = F / d;
//    values[2][2] = K / d;
//
//    // Multiply the b
//    T b0 = - values[0][0] * values[0][3] - values[0][1] * values[1][3] - values[0][2] * values[2][3];
//    T b1 = - values[1][0] * values[0][3] - values[1][1] * values[1][3] - values[1][2] * values[2][3];
//    T b2 = - values[2][0] * values[0][3] - values[2][1] * values[1][3] - values[2][2] * values[2][3];
//
//    values[0][3] = b0;
//    values[1][3] = b1;
//    values[2][3] = b2;
//
//    return *this;
//}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
T
util::matrix<S,T>::determinant (void) const
{
    return util::determinant (*this);
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::matrix<S,T>
util::matrix<S,T>::inverse (void) const
{
    return util::inverse (*this);
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
matrix<S,T>
util::transposed (const matrix<S,T> &m)
{
    util::matrix<S,T> res;

    for (size_t y = 0; y < S; ++y)
        for (size_t x = 0; x < S; ++x)
            res[y][x] = m[x][y];

    return res;
}


//-----------------------------------------------------------------------------
template matrix3f util::transposed (const matrix3f&);
template matrix4f util::transposed (const matrix4f&);


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
matrix<S,T>
matrix<S,T>::operator* (const matrix<S,T> &rhs) const
{
    matrix<S,T> m;

    for (unsigned row = 0; row < S; ++row) {
        for (unsigned col = 0; col < S; ++col) {
            m.values[row][col] = T {0};

            for (unsigned inner = 0; inner < S; ++inner)
                m.values[row][col] += values[row][inner] * rhs.values[inner][col];
        }
    }

    return m;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
matrix<S,T>&
matrix<S,T>::operator*=(const matrix<S,T> &rhs)
{
    return *this = *this * rhs;
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
vector<S,T>
matrix<S,T>::operator* (const vector<S,T> &rhs) const
{
    vector<S,T> out;

    for (size_t i = 0; i < S; ++i)
        out[i] = dot (rhs, values[i]);

    return out;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
point<S,T>
matrix<S,T>::operator* (const point<S,T> &rhs) const
{
    point<S,T> out;

    for (size_t i = 0; i < S; ++i)
        out[i] = dot (rhs, values[i]);

    return out;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
bool
matrix<S,T>::is_affine (void) const
{
    for (size_t i = 0; i < S - 1; ++i)
        if (!exactly_zero (values[S-1][i]))
            return false;

    return exactly_equal (values[S-1][S-1], T{1});
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
matrix4<T>
matrix<S,T>::ortho (T left, T right,
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
template <size_t S, typename T>
matrix4<T>
matrix<S,T>::ortho2D (T left, T right,
                      T bottom, T top)
{
    return ortho (left, right, bottom, top, -1, 1);
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
matrix4<T>
matrix<S,T>::perspective (T fov, T aspect, range<T> Z)
{
    CHECK_GE (Z.min, 0);
    CHECK_GE (Z.max, 0);

    T f = 1 / std::tan (fov / 2);

    T  x = f / aspect;
    T  y = f;
    T z1 =     (Z.max + Z.min) / (Z.min - Z.max);
    T z2 = (2 * Z.max * Z.min) / (Z.min - Z.max);

    return { {
        {  x,  0,  0,  0 },
        {  0,  y,  0,  0 },
        {  0,  0, z1, z2 },
        {  0,  0, -1,  0 }
    } };
}


//-----------------------------------------------------------------------------
// Emulates gluLookAt
template <size_t S, typename T>
matrix4<T>
matrix<S,T>::look_at (util::point<3,T> eye,
                      util::point<3,T> target,
                      util::vector<3,T> up)
{
    CHECK (is_normalised (up));

    auto forward = normalised (eye.to (target));
    auto side    = normalised (cross (forward, up));
    up = cross (side, forward);

    auto rot = util::matrix4<T> {{
        { side[0], up[0], -forward[0], 0 },
        { side[1], up[1], -forward[1], 0 },
        { side[2], up[2], -forward[2], 0 },
        { 0, 0, 0, 1 }
    }};

    return util::matrix4<T>::translation (-eye.template as<vector> ()) * rot;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
matrix4<T>
matrix<S,T>::translation (util::vector<2,T> v)
{
    return translation ({v.x, v.y, 0});
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
matrix4<T>
matrix<S,T>::translation (util::vector<3,T> v)
{
    return { {
        { 1.f, 0.f, 0.f, v.x },
        { 0.f, 1.f, 0.f, v.y },
        { 0.f, 0.f, 1.f, v.z },
        { 0.f, 0.f, 0.f, 1.f },
    } };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
matrix4<T>
matrix<S,T>::scale (T mag)
{
    return scale (vector<3,T> (mag));
}

//-----------------------------------------------------------------------------
template <size_t S, typename T>
matrix4<T>
matrix<S,T>::scale (util::vector<3,T> v)
{
    return { {
        { v.x, 0.f, 0.f, 0.f },
        { 0.f, v.y, 0.f, 0.f },
        { 0.f, 0.f, v.z, 0.f },
        { 0.f, 0.f, 0.f, 1.f }
    } };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
matrix4<T>
matrix<S,T>::rotation (T angle, util::vector<3,T> about)
{
    CHECK (is_normalised (about));

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
template <size_t S, typename T>
const matrix<S,T>
matrix<S,T>::ZEROES { 0 };


//-----------------------------------------------------------------------------
template struct util::matrix<2,float>;
template struct util::matrix<2,double>;

template struct util::matrix<3,float>;
template struct util::matrix<3,double>;

template struct util::matrix<4,float>;
template struct util::matrix<4,double>;


///////////////////////////////////////////////////////////////////////////////
// Uses the algorithm from:
//    "Extracting Euler Angles from a Rotation Matrix" by
//    Mike Day, Insomniac Games.
template <size_t S, typename T>
util::vector<3,T>
util::to_euler (const matrix<S,T> &m)
{
    static_assert (S == 3 || S == 4, "only defined for 3d affine transforms");

    const auto theta0 = std::atan2 (m[2][1], m[2][2]);

    const auto c1 = std::hypot (m[0][0], m[1][0]);
    const auto theta1 = std::atan2 (-m[2][0], c1);

    const auto s0 = std::sin(theta0);
    const auto c0 = std::cos(theta0);
    const auto theta2 = std::atan2(
        s0 * m[0][2] - c0 * m[0][1],
        c0 * m[1][1] - s0 * m[1][2]
    );

    return { theta0, theta1, theta2 };
}


//-----------------------------------------------------------------------------
template util::vector<3,float> util::to_euler (const matrix<3,float>&);
template util::vector<3,float> util::to_euler (const matrix<4,float>&);


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
std::ostream&
util::operator<< (std::ostream &os, const matrix<S,T> &m)
{
    os << "{ ";

    for (size_t i = 0; i < S; ++i) {
        os << "{ ";
        std::copy (m[i], m[i]+S, util::infix_iterator<float> (os, ", "));
        os << ((i == S - 1) ? " }" : " }, ");
    }

    return os << " }";
}


//-----------------------------------------------------------------------------
template std::ostream& util::operator<< (std::ostream&, const matrix<4,float>&);
template std::ostream& util::operator<< (std::ostream&, const matrix<4,double>&);
