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


#include "plane.hpp"

#include "debug.hpp"

//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::plane<S,T>::plane (util::point<S,T> _p,
                         util::vector<S,T> _n):
    p (_p),
    n (_n)
{
    CHECK_EQ (n.magnitude2 (), 1);
}


//-----------------------------------------------------------------------------
template struct util::plane<2,float>;
template struct util::plane<3,float>;
