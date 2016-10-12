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
#include "./vector.hpp"

#include <cmath>

//-----------------------------------------------------------------------------
using util::quaternion;


//-----------------------------------------------------------------------------
template<> const quaternion<4,  float> quaternion<4,  float>::IDENTITY = { 1, 0, 0, 0 };
template<> const quaternion<4, double> quaternion<4, double>::IDENTITY = { 1, 0, 0, 0 };


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
quaternion<S,T>
quaternion<S,T>::angle_axis (const T radians, const vector<3,T> axis)
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
quaternion<S,T>::from_euler (vector<3,T> angles)
{
    auto half = angles / 2;

    auto c = cos (half);
    auto s = sin (half);

    return {
        c.x * c.y * c.z - s.x * s.y * s.z,
        s.x * c.y * c.z + c.x * s.y * s.z,
        c.x * s.y * c.z - s.x * c.y * s.z,
        c.x * c.y * s.z + s.x * s.y * c.z,
    };
}


///////////////////////////////////////////////////////////////////////////////
// vector-to-vector rotation algorithm from:
//     http://lolengine.net/blog/2014/02/24/quaternion-from-two-vectors-final
template <size_t S, typename T>
quaternion<S,T>
quaternion<S,T>::from_to (const vector<3,T> u, const vector<3,T> v)
{
    CHECK (is_normalised (u));
    CHECK (is_normalised (v));

    auto norm_u_norm_v = std::sqrt(dot(u, u) * dot(v, v));
    auto real_part = norm_u_norm_v + dot(u, v);
    util::vector<3,T> w;

    if (real_part < 1.e-6f * norm_u_norm_v)
    {
        /* If u and v are exactly opposite, rotate 180 degrees
         * around an arbitrary orthogonal axis. Axis normalisation
         * can happen later, when we normalise the quaternion. */
        real_part = 0.0f;
        w = std::abs(u.x) > std::abs(u.z) ?
            util::vector3<T> (-u.y, u.x, 0.f) :
            util::vector3<T> (0.f, -u.z, u.y);
    }
    else
    {
        /* Otherwise, build quaternion the standard way. */
        w = cross(u, v);
    }

    return normalised (util::quaternion<4,T> (real_part, w.x, w.y, w.z));
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
quaternion<4,T>
util::conjugate (quaternion<4,T> q)
{
    return { q.w, -q.x, -q.y, -q.z };
}


//-----------------------------------------------------------------------------
template quaternion<4,float> util::conjugate (quaternion<4,float>);


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
// https://gamedev.stackexchange.com/questions/28395/rotating-vector3-by-a-quaternion
template <typename T>
util::vector3<T>
util::rotate (vector3<T> v, quaternion<4,T> q)
{
    CHECK (is_normalised (v));

#if 1
    util::vector3<T> u { q.x, q.y, q.z };
    return v + 2 * cross (u, cross (u, v) + q.w * v);
#else
    // Verbosely:
    quaternionf p { 0, v.x, v.y, v.z };
    auto p_ = q * p * conjugate (q);
    return { p_.x, p_.y, p_.z };
#endif
}


//-----------------------------------------------------------------------------
template util::vector3f util::rotate (util::vector3f, util::quaternionf);
template util::vector3d util::rotate (util::vector3d, util::quaterniond);


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
quaternion<S,T>
quaternion<S,T>::look (vector<3,T> fwd, vector<3,T> up)
{
    CHECK (is_normalised (fwd));
    CHECK (is_normalised (up));

    constexpr util::vector3<T> RH_FWD { 0, 0, -1 };
    constexpr util::vector3<T> RH_UP  { 0, 1,  0 };

    // rotate the right-handed fwd to face the given fwd
    auto q1 = from_to (RH_FWD, fwd);

    // can't retain the up direction if fwd is the same as up
    if (norm2 (cross (fwd, up)) < 1e-6)
        return q1;

    // find the right-handed up rotated to the given fwd
    auto new_up = rotate (RH_UP, q1);

    // rotate first to the new forward, then align the right handed up with
    // the given up.
    return from_to (new_up, up) * q1;
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
