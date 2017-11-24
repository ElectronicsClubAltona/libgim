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

using util::matrix;


///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
//template <std::size_t S, typename T>
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
template <std::size_t Rows, std::size_t Cols, typename T>
T
util::matrix<Rows,Cols,T>::determinant (void) const
{
    return util::determinant (*this);
}


//-----------------------------------------------------------------------------
template <std::size_t Rows, std::size_t Cols, typename T>
util::matrix<Rows,Cols,T>
util::matrix<Rows,Cols,T>::inverse (void) const
{
    return util::inverse (*this);
}


///////////////////////////////////////////////////////////////////////////////
template <std::size_t Rows, std::size_t Cols, typename T>
matrix<Cols,Rows,T>
util::transposed (const matrix<Rows,Cols,T> &m)
{
    util::matrix<Cols,Rows,T> res;

    for (std::size_t y = 0; y < Rows; ++y)
        for (std::size_t x = 0; x < Cols; ++x)
            res[y][x] = m[x][y];

    return res;
}


//-----------------------------------------------------------------------------
template util::matrix3f util::transposed (const matrix3f&);
template util::matrix4f util::transposed (const matrix4f&);


///////////////////////////////////////////////////////////////////////////////
template <std::size_t Rows, std::size_t Cols, typename T>
bool
matrix<Rows,Cols,T>::is_affine (void) const
{
    if (Rows != Cols)
        return false;

    for (std::size_t i = 0; i < Rows - 1; ++i)
        if (!exactly_zero (values[Rows-1][i]))
            return false;

    return exactly_equal (values[Rows-1][Rows-1], T{1});
}


//-----------------------------------------------------------------------------
template <typename T>
util::matrix4<T>
util::ortho (T left,   T right,
             T bottom, T top,
             T near,   T far)
{
    CHECK_GT (far, near);

    T tx = - (right + left) / (right - left);
    T ty = - (top + bottom) / (top - bottom);
    T tz = - (far + near)   / (far - near);

    T rl = 2 / (right - left);
    T tb = 2 / (top - bottom);
    T fn = 2 / (far - near);

    return {{
        { rl,  0,  0, tx },
        {  0, tb,  0, ty },
        {  0,  0, fn, tz },
        {  0,  0,  0,  1 },
    }};
}


//-----------------------------------------------------------------------------
template <typename T>
util::matrix4<T>
util::ortho2D (T left, T right,
               T bottom, T top)
{
    return ortho (left, right, bottom, top, -1, 1);
}


//-----------------------------------------------------------------------------
template <typename T>
util::matrix4<T>
util::perspective (T fov, T aspect, range<T> Z)
{
    CHECK_GE (Z.lo, 0);
    CHECK_GE (Z.hi, 0);

    T f = 1 / std::tan (fov / 2);

    T  x = f / aspect;
    T  y = f;
    T z1 =     (Z.hi + Z.lo) / (Z.lo - Z.hi);
    T z2 = (2 * Z.hi * Z.lo) / (Z.lo - Z.hi);

    return { {
        {  x,  0,  0,  0 },
        {  0,  y,  0,  0 },
        {  0,  0, z1, z2 },
        {  0,  0, -1,  0 }
    } };
}


//-----------------------------------------------------------------------------
// Emulates gluLookAt
//
// Translates the viewpoint to the origin, then rotates the world to point
// along eye to centre (negative-Z).
// Implemented for right handed world coordinates.
//
// Assumes 'up' is normalised.
template <typename T>
util::matrix4<T>
util::look_at (const util::point<3,T> eye,
               const util::point<3,T> centre,
               const util::vector<3,T> up)
{
    CHECK (is_normalised (up));

    const auto f = normalised (centre - eye);
    const auto s = normalised (cross (f, up));
    const auto u = cross (s, f);

    const util::matrix4<T> rot {{
        { s.x, s.y, s.z, 0 },
        { u.x, u.y, u.z, 0 },
        {-f.x,-f.y,-f.z, 0 },
        {   0,   0,   0, 1 },
    }};

    return rot * util::translation<T> (0-eye);
}

template util::matrix4f util::look_at (util::point3f, util::point3f, util::vector3f);


//-----------------------------------------------------------------------------
template <typename T>
util::matrix4<T>
util::translation (util::vector<3,T> v)
{
    return { {
        { 1.f, 0.f, 0.f, v.x },
        { 0.f, 1.f, 0.f, v.y },
        { 0.f, 0.f, 1.f, v.z },
        { 0.f, 0.f, 0.f, 1.f },
    } };
}


template util::matrix4f util::translation (util::vector3f);


//-----------------------------------------------------------------------------
template <typename T>
util::matrix4<T>
util::scale (T mag)
{
    return scale (vector<3,T> (mag));
}

//-----------------------------------------------------------------------------
template <typename T>
util::matrix4<T>
util::scale (util::vector<3,T> v)
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
util::matrix4<T>
util::rotation (T angle, util::vector<3,T> about)
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

template util::matrix4f util::rotation (float, util::vector3f);

//-----------------------------------------------------------------------------
template struct util::matrix<2,2,float>;
template struct util::matrix<2,2,double>;

template struct util::matrix<3,3,float>;
template struct util::matrix<3,3,double>;

template struct util::matrix<4,4,float>;
template struct util::matrix<4,4,double>;


///////////////////////////////////////////////////////////////////////////////
// Uses the algorithm from:
//    "Extracting Euler Angles from a Rotation Matrix" by
//    Mike Day, Insomniac Games.
template <std::size_t Rows, std::size_t Cols, typename T>
util::vector<3,T>
util::to_euler (const matrix<Rows,Cols,T> &m)
{
    static_assert (Rows == Cols && (Rows == 3 || Rows == 4),
                   "only defined for 3d affine transforms");

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
template util::vector<3,float> util::to_euler (const matrix<3,3,float>&);
template util::vector<3,float> util::to_euler (const matrix<4,4,float>&);


///////////////////////////////////////////////////////////////////////////////
template <std::size_t Rows, std::size_t Cols, typename T>
std::ostream&
util::operator<< (std::ostream &os, const matrix<Rows,Cols,T> &m)
{
    os << "{ ";

    for (std::size_t i = 0; i < Rows; ++i) {
        os << "{ ";
        std::copy (std::cbegin (m[i]), std::cend (m[i]), util::infix_iterator<float> (os, ", "));
        os << ((i == Rows - 1) ? " }" : " }, ");
    }

    return os << " }";
}


//-----------------------------------------------------------------------------
template std::ostream& util::operator<< (std::ostream&, const matrix<4,4,float>&);
template std::ostream& util::operator<< (std::ostream&, const matrix<4,4,double>&);
