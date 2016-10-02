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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#include "./ray.hpp"

#include "./iostream.hpp"
#include "./ops.hpp"

#include "../debug.hpp"

using util::geom::ray;


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
ray<S,T>::ray (util::point<S,T> _origin,
               util::vector<S,T> _direction):
    origin (_origin),
    direction (_direction)
{
    CHECK (is_normalised (direction));
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
ray<S,T>
ray<S,T>::make (util::point<S,T> origin,
                util::point<S,T> target)
{
    return {
        origin,
        normalised (target - origin)
    };
}


///////////////////////////////////////////////////////////////////////////////
/// returns the distance along the ray in a ray-plane intersection
///
/// returns inf if parallel
/// returns 0   if corayar
template <size_t S, typename T>
T
ray<S,T>::intersect (plane<S,T> q) const
{
    return dot (q.p - origin, q.n) / dot (direction, q.n);
}


///----------------------------------------------------------------------------
/// returns the distance from origin to AABB intersection
///
/// returns NaN on miss
/// returns NaN if behind
template <size_t S, typename T>
T
ray<S,T>::intersect (AABB<S,T> r) const
{
    auto t1 = (r.p0 - origin) / direction;
    auto t2 = (r.p1 - origin) / direction;

    auto vmin = min (t1, t2);
    auto vmax = max (t1, t2);

    auto tmin = max (vmin);
    auto tmax = min (vmax);

    // closest intersection is behind us
    if (tmax < 0)
        return std::numeric_limits<T>::quiet_NaN ();

    // missed intersection
    if (tmin > tmax)
        return std::numeric_limits<T>::quiet_NaN ();

    return tmin;
}


///----------------------------------------------------------------------------
/// returns the smallest distance from ray origin to a sphere intersection
///
/// returns NaN on miss
/// returns NaN if behind
template <size_t S, typename T>
T
ray<S,T>::intersect (sphere<S,T> s) const
{
    T b = dot (direction, origin - s.centre);
    T c = dot (origin - s.centre, origin - s.centre) - s.radius * s.radius;

    T D = b * b - c;
    if (D < 0)
        return std::numeric_limits<T>::quiet_NaN ();

    auto t_ = std::sqrt (D);
    auto t0 = -b + t_;
    auto t1 = -b - t_;

    return t1 >= 0 ? t1 :
           t0 >= 0 ? t0 :
           std::numeric_limits<T>::quiet_NaN ();
}


///////////////////////////////////////////////////////////////////////////////
/// returns the closest parameter along the ray to a given point
template <size_t S, typename T>
T
ray<S,T>::closest (point<S,T> q) const
{
    // project the origin-point difference onto the direction
    return dot (origin - q, direction);
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::point<S,T>
ray<S,T>::at (T t) const
{
    return origin + direction * t;
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
std::ostream&
util::geom::operator<< (std::ostream &os, ray<S,T> r)
{
    return os << "ray(" << r.origin << ',' << r.direction << ')';
}

template std::ostream& util::geom::operator<< (std::ostream&, ray<3,float>);
template std::ostream& util::geom::operator<< (std::ostream&, ray<3,double>);


///////////////////////////////////////////////////////////////////////////////
template struct util::geom::ray<2,float>;
template struct util::geom::ray<3,float>;
