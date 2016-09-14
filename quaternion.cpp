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


#include "./quaternion.hpp"

#include "./debug.hpp"
#include "./maths.hpp"
#include "./vector.hpp"

#include <iostream>

//-----------------------------------------------------------------------------
using util::quaternion;


//-----------------------------------------------------------------------------
template<> const quaternion<4,  float> quaternion<4,  float>::IDENTITY = { 1, 0, 0, 0 };
template<> const quaternion<4, double> quaternion<4, double>::IDENTITY = { 1, 0, 0, 0 };


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
quaternion<S,T>
quaternion<S,T>::rotation (const T radians, const vector<3,T> axis)
{
    CHECK (is_normalised (axis));

    auto w   = std::cos (radians / 2);
    auto xyz = std::sin (radians / 2) * axis;

    return {
        w, xyz.x, xyz.y, xyz.z
    };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
quaternion<S,T>
quaternion<S,T>::rotation (const vector<3,T> src, const vector<3,T> dst)
{
    auto v = util::cross (src, dst);

    return {
        std::acos (dot (src, dst)),
        v.x,
        v.y,
        v.z
    };
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
quaternion<S,T>
util::operator* (const quaternion<S,T> a, const quaternion<S,T> b)
{
    return {
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
        a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
        a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
        a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
    };
}

template quaternion<4,float> util::operator* (quaternion<4,float>, quaternion<4,float>);


//-----------------------------------------------------------------------------
template <size_t S, typename T>
quaternion<S,T>
util::operator/ (const quaternion<S,T> a, const quaternion<S,T> b)
{
    CHECK (is_normalised (a));
    CHECK (is_normalised (b));

    return quaternion<S,T> {
          a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z,
        - a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
        - a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
        - a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
    };
}

template quaternion<4,float> util::operator/ (quaternion<4,float>, quaternion<4,float>);


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
util::matrix4<T>
quaternion<S, T>::as_matrix (void) const
{
    CHECK (is_normalised (*this));

    const T wx = this->w * this->x, wy = this->w * this->y, wz = this->w * this->z;
    const T xx = this->x * this->x, xy = this->x * this->y, xz = this->x * this->z;
    const T yy = this->y * this->y, yz = this->y * this->z, zz = this->z * this->z;

    return { {
        { 1 - 2 * (yy + zz),     2 * (xy - wz),     2 * (xz + wy), 0 },
        {     2 * (xy + wz), 1 - 2 * (xx + zz),     2 * (yz - wx), 0 },
        {     2 * (xz - wy),     2 * (yz + wx), 1 - 2 * (xx + yy), 0 },
        {                 0,                 0,                 0, 1 }
    } };
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
std::ostream&
util::operator<< (std::ostream &os, const quaternion<S,T> q)
{
    return os << "[" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << "]";
}


//-----------------------------------------------------------------------------
template std::ostream& util::operator<< (std::ostream&, quaternion<4,float>);
template std::ostream& util::operator<< (std::ostream&, quaternion<4,double>);


///////////////////////////////////////////////////////////////////////////////
namespace util { namespace debug {
    template <size_t S, typename T>
    struct validator<quaternion<S,T>> {
        static bool is_valid (const quaternion<S,T> &q)
        {
            return is_normalised (q);
        }
    };
} }


//-----------------------------------------------------------------------------
template bool util::debug::is_valid(const quaternion<4,float>&);
template bool util::debug::is_valid(const quaternion<4,double>&);


///////////////////////////////////////////////////////////////////////////////
template struct util::quaternion<4,float>;
template struct util::quaternion<4,double>;
