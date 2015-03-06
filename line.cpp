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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#include "line.hpp"

#include "debug.hpp"


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::line<S,T>::line (util::point<S,T> _p,
                       util::vector<S,T> _d):
    p (_p),
    d (_d)
{
    CHECK_EQ (d.magnitude2 (), 1);
}


///----------------------------------------------------------------------------
/// returns the distance along the line in a line-plane intersection
///
/// returns inf if parallel
/// returns 0   if colinear
template <size_t S, typename T>
T
util::line<S,T>::intersect (plane<S,T> q) const
{
    return dot (q.p - p, q.n) / dot (d, q.n);
}


///----------------------------------------------------------------------------
/// returns the distance from origin to AABB intersection
///
/// returns NaN on miss
/// returns -ve if behind
template <size_t S, typename T>
T
util::line<S,T>::intersect (region<S,T> r) const
{
    auto t1 = (r.base () - p) / d;
    auto t2 = (r.away () - p) / d;

    auto vmin = min (t1, t2);
    auto vmax = min (t1, t2);

    auto tmin = min (vmax);
    auto tmax = max (vmin);

    if (tmax < 0)
        return tmax;

    if (tmin > tmax)
        return std::numeric_limits<T>::quiet_NaN ();

    return tmin;
}


///----------------------------------------------------------------------------
/// returns the closest parameter along the line to a given point
template <size_t S, typename T>
T
util::line<S,T>::closest (point<S,T> q) const
{
    // project the origin-point difference onto the direction
    return dot (p - q, d);
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::point<S,T>
util::line<S,T>::at (T t) const
{
    return p + d * t;
}


//-----------------------------------------------------------------------------
template struct util::line<2,float>;
template struct util::line<3,float>;
