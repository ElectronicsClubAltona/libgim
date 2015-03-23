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


#ifdef __UTIL_EXTENT_IPP
#error
#endif

#define __UTIL_EXTENT_IPP

#include <algorithm>

//-----------------------------------------------------------------------------
template <size_t S, typename T>
template <typename U>
util::extent<S,U>
util::extent<S,T>::cast (void) const
{
    util::extent<S,U> out;
    std::copy (std::begin (this->data), std::end (this->data), std::begin (out.data));
    return out;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
template <typename U>
U
util::extent<S,T>::aspect (void) const
{
    return static_cast<U> (this->w) / this->h;
}
