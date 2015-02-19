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
/// returns the distance alone the line in a line-plane intersection
///
/// returns inf if parallel
/// returns 0   if colinear
template <size_t S, typename T>
T
util::line<S,T>::intersect (plane<S,T> rhs) const
{
    return dot (rhs.p - p, rhs.n) / dot (d, rhs.n);
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
