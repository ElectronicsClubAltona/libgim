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

#if defined(CRUFT_UTIL_QUATERNION_IPP)
#error
#endif

#define CRUFT_UTIL_QUATERNION_IPP

#include <cmath>


///////////////////////////////////////////////////////////////////////////////
template <typename T>
constexpr
util::quaternion<T>
util::quaternion<T>::identity (void)
{
    return { 1, 0, 0, 0 };
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
constexpr
T
util::norm2 (quaternion<T> q)
{
    return q.w * q.w +
           q.x * q.x +
           q.y * q.y +
           q.z * q.z;
}


//-----------------------------------------------------------------------------
template <typename T>
constexpr
T
util::norm (quaternion<T> q)
{
    return std::sqrt (norm2 (q));
}


//-----------------------------------------------------------------------------
template <typename T>
constexpr
bool
util::is_normalised (quaternion<T> q)
{
    return almost_equal (T{1}, norm2 (q));
}


//-----------------------------------------------------------------------------
template <typename T>
constexpr
util::quaternion<T>
util::normalised (quaternion<T> q)
{
    return q / norm (q);
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
constexpr
util::quaternion<T>
util::operator/ (quaternion<T> q, T t)
{
    return { q.w / t, q.x / t, q.y / t, q.z / t };
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
constexpr
bool
util::operator== (quaternion<T> a, quaternion<T> b)
{
    return exactly_equal (a.w, b.w) &&
           exactly_equal (a.x, b.x) &&
           exactly_equal (a.y, b.y) &&
           exactly_equal (a.z, b.z);
}
