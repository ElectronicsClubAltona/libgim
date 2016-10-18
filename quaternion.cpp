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
 * Copyright 2011-2016 Danny Robson <danny@nerdcruft.net>
 */


#include "./quaternion.hpp"

#include "./debug.hpp"
#include "./vector.hpp"

#include <cmath>


///////////////////////////////////////////////////////////////////////////////
using util::quaternion;


///////////////////////////////////////////////////////////////////////////////
template <typename T>
quaternion<T>
quaternion<T>::angle_axis (const T radians, const vector<3,T> axis)
{
    CHECK (is_normalised (axis));

    auto w   = std::cos (radians / 2);
    auto xyz = std::sin (radians / 2) * axis;

    return {
        w, xyz.x, xyz.y, xyz.z
    };
}


//-----------------------------------------------------------------------------
template <typename T>
quaternion<T>
quaternion<T>::from_euler (vector<3,T> angles)
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
template <typename T>
quaternion<T>
quaternion<T>::from_to (const vector<3,T> u, const vector<3,T> v)
{
    CHECK (is_normalised (u));
    CHECK (is_normalised (v));

#if 0
    // Naive:
    auto cos_theta = dot (u, v);
    auto angle = std::acos (cos_theta);
    auto axis = normalised (cross (u, v));
    return angle_axis (angle, axis);
#elif 1
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

    return normalised (util::quaternion<T> {real_part, w.x, w.y, w.z});
#endif
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
quaternion<T>
util::conjugate (quaternion<T> q)
{
    return { q.w, -q.x, -q.y, -q.z };
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
quaternion<T>
util::operator* (const quaternion<T> a, const quaternion<T> b)
{
    return {
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
        a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
        a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
        a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
    };
}


//-----------------------------------------------------------------------------
template <typename T>
quaternion<T>&
util::operator*= (quaternion<T> &a, const quaternion<T> b)
{
    return a = a * b;
}


//-----------------------------------------------------------------------------
template <typename T>
quaternion<T>
util::operator/ (const quaternion<T> a, const quaternion<T> b)
{
    CHECK (is_normalised (a));
    CHECK (is_normalised (b));

    return quaternion<T> {
          a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z,
        - a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
        - a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
        - a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
    };
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
util::matrix4<T>
quaternion<T>::as_matrix (void) const
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
util::rotate (vector3<T> v, quaternion<T> q)
{
    CHECK (is_normalised (v));
    CHECK (is_normalised (q));

#if 0
    // Naive:
    quaternion<T> p { 0, v.x, v.y, v.z };
    auto p_ = q * p * conjugate (q);
    return { p_.x, p_.y, p_.z };
#elif 1
    // This code actually matches the stackexchange link, but is longer than
    // the code below it (which also actually works)...
    const util::vector3<T> u { q.x, q.y, q.z };
    const auto s = q.w;

    return 2 * dot (u, v) * u
         + (s * s - dot (u, u)) * v
         + 2 * s * cross (u, v);
#elif 0
    // I have no idea where this code is from or how it was derived...
    util::vector3<T> u { q.x, q.y, q.z };
    return v + 2 * cross (u, cross (u, v) + q.w * v);
#endif
}


///////////////////////////////////////////////////////////////////////////////
// based on the implementation at:
// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
template <typename T>
quaternion<T>
quaternion<T>::look (vector<3,T> fwd, vector<3,T> up)
{
    CHECK (is_normalised (fwd));
    CHECK (is_normalised (up));

    constexpr util::vector3<T> FWD { 0, 0, -1 };
    constexpr util::vector3<T> UP  { 0, 1, 0 };

    // find the rotation from the world fwd to the object fwd
    auto q1 = from_to (FWD, fwd);

    // orthogonalise the up vector
    auto right = cross (fwd, up);
    auto orthup = normalised (cross (right, fwd));

    // recompute the up vector in object space
    auto newup = rotate (UP, q1);

    // find rotation from object up to world up
    auto q2 = from_to (newup, orthup);

    return q2 * q1;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
bool
util::almost_equal (quaternion<T> a, quaternion<T> b)
{
    return almost_equal (a.w, b.w) &&
           almost_equal (a.x, b.x) &&
           almost_equal (a.y, b.y) &&
           almost_equal (a.z, b.z);
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
std::ostream&
util::operator<< (std::ostream &os, const quaternion<T> q)
{
    return os << "[" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << "]";
}


///////////////////////////////////////////////////////////////////////////////
namespace util::debug {
    template <typename T>
    struct validator<quaternion<T>> {
        static constexpr
        bool
        is_valid (const quaternion<T> &q)
        {
            return is_normalised (q);
        }
    };
}


///////////////////////////////////////////////////////////////////////////////
#define INSTANTIATE(T)                                                          \
template util::vector3<T> util::rotate (util::vector3<T>, util::quaternion<T>); \
template quaternion<T> util::conjugate (quaternion<T>);                         \
template quaternion<T> util::operator* (quaternion<T>, quaternion<T>);          \
template quaternion<T>& util::operator*= (quaternion<T>&, quaternion<T>);       \
template quaternion<T> util::operator/ (quaternion<T>, quaternion<T>);          \
template bool util::almost_equal (util::quaternion<T>, util::quaternion<T>);    \
template std::ostream& util::operator<< (std::ostream&, quaternion<T>);         \
template bool util::debug::is_valid(const quaternion<T>&);                      \
template struct util::quaternion<T>;

INSTANTIATE(float)
INSTANTIATE(double)
