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


#include "quaternion.hpp"

#include "debug.hpp"
#include "maths.hpp"


//-----------------------------------------------------------------------------
using util::quaternion;
using util::matrix4;


//-----------------------------------------------------------------------------
template<> const quaternion< float> quaternion< float>::IDENTITY = { 1, 0, 0, 0 };
template<> const quaternion<double> quaternion<double>::IDENTITY = { 1, 0, 0, 0 };


///////////////////////////////////////////////////////////////////////////////
template <typename T>
quaternion<T>::quaternion (T _a, T _b, T _c, T _d):
    a (_a),
    b (_b),
    c (_c),
    d (_d)
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
quaternion<T>::quaternion (T _a):
    a (_a),
    b (T{}),
    c (T{}),
    d (T{})
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
quaternion<T>::quaternion ():
    quaternion (T{}, T{}, T{}, T{})
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
quaternion<T>::quaternion (vector3<T> v):
    quaternion (0, v.x, v.y, v.z)
{ ; }


///////////////////////////////////////////////////////////////////////////////
template <typename T>
quaternion<T>
quaternion<T>::rotation (const T radians, const vector<3,T> axis) {
    CHECK (axis.is_normalised ());

    return {
        std::cos (radians / 2),
        std::sin (radians / 2) * axis.x,
        std::sin (radians / 2) * axis.y,
        std::sin (radians / 2) * axis.z
    };
}


//-----------------------------------------------------------------------------
template <typename T>
quaternion<T>
quaternion<T>::rotation (vector<3,T> src, vector<3,T> dst) {
    auto v = util::cross (src, dst);

    return {
        std::acos (dot (src, dst)),
        v.x,
        v.y,
        v.z
    };
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
T
quaternion<T>::norm2 (void) const
{
    return a * a + b * b + c * c + d * d;
}


//-----------------------------------------------------------------------------
template <typename T>
T
quaternion<T>::norm (void) const
{
    return std::sqrt (norm2 ());
}


//-----------------------------------------------------------------------------
template <typename T>
quaternion<T>
quaternion<T>::normalised (void) const
{
    return *this / norm ();
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
quaternion<T>
quaternion<T>::operator- (void) const
{
    return { -a, -b, -c, -d };
}


//-----------------------------------------------------------------------------
template <typename T>
quaternion<T>
quaternion<T>::conjugate (void) const
{
    return { a, -b, -c, -d };
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
quaternion<T>
quaternion<T>::operator+ (const quaternion<T> q) const
{
    return {
        a + q.a,
        b + q.b,
        c + q.c,
        d + q.d
    };
}


//-----------------------------------------------------------------------------
template <typename T>
quaternion<T>
quaternion<T>::operator- (const quaternion<T> q) const
{
    return {
        a - q.a,
        b - q.b,
        c - q.c,
        d - q.d
    };
}


//-----------------------------------------------------------------------------
template <typename T>
quaternion<T>
quaternion<T>::operator* (const quaternion<T> q) const {
    return {
        a * q.a - b * q.b - c * q.c - d * q.d,
        a * q.b + b * q.a + c * q.d - d * q.c,
        a * q.c - b * q.d + c * q.a + d * q.b,
        a * q.d + b * q.c - c * q.b + d * q.a,
    };
}


//-----------------------------------------------------------------------------
template <typename T>
quaternion<T>
quaternion<T>::operator/ (const quaternion<T> q) const
{
    auto n = q.norm2 ();

    return {
        (  a * q.a + b * q.b + c * q.c + d * q.d) / n,
        (- a * q.b + b * q.a + c * q.d - d * q.c) / n,
        (- a * q.c - b * q.d + c * q.a + d * q.b) / n,
        (- a * q.d + b * q.c - c * q.b + d * q.a) / n
    };
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
quaternion<T>
quaternion<T>::operator+ (const T t) const
{
    return { a + t, b, c, d };
}


//-----------------------------------------------------------------------------
template <typename T>
quaternion<T>
quaternion<T>::operator- (const T t) const
{
    return { a - t, b, c, d };
}


//-----------------------------------------------------------------------------
template <typename T>
quaternion<T>
quaternion<T>::operator* (const T t) const
{
    return { a * t, b, c, d };
}


//-----------------------------------------------------------------------------
template <typename T>
quaternion<T>
quaternion<T>::operator/ (const T t) const
{
    return { a / t, b, c, d };
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
bool
quaternion<T>::operator== (const quaternion<T> rhs) const
{
    return almost_equal (a, rhs.a) &&
           almost_equal (b, rhs.b) &&
           almost_equal (c, rhs.c) &&
           almost_equal (d, rhs.d);
}


//-----------------------------------------------------------------------------
template <typename T>
bool
quaternion<T>::operator!= (const quaternion<T> rhs) const
{
    return !(*this == rhs);
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
matrix4<T>
quaternion<T>::rotation_matrix (void) const
{
    CHECK_EQ (T{1}, norm ());

    const T wx = w * x, wy = w * y, wz = w * z;
    const T xx = x * x, xy = x * y, xz = x * z;
    const T yy = y * y, yz = y * z, zz = z * z;

    return { {
        { 1 - 2 * (yy - zz),     2 * (xy - wz),     2 * (xz + wy), 0 },
        {     2 * (xy + wz), 1 - 2 * (xx - zz),     2 * (yz - wx), 0 },
        {     2 * (xz - wy),     2 * (yz + wx), 1 - 2 * (xx - yy), 0 },
        {                 0,                 0,                 0, 1 }
    } };
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
std::ostream&
util::operator<< (std::ostream &os, quaternion<T> q)
{
    os << q.w << ' ' << q.x << "i " << q.y << "j " << q.z << 'k';
    return os;
}


///////////////////////////////////////////////////////////////////////////////
template struct util::quaternion<float>;
template struct util::quaternion<double>;

template std::ostream& util::operator<< (std::ostream&, quaternion<float>);
template std::ostream& util::operator<< (std::ostream&, quaternion<double>);
