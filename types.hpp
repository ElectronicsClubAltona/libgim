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
 * Copyright 2011-2014 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_TYPES_HPP
#define __UTIL_TYPES_HPP

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <stdexcept>

//-----------------------------------------------------------------------------
/// Returns the number of elements of a statically allocated array
template <typename T, size_t N>  
constexpr size_t elems(const T (&)[N])
    { return N; }


//-----------------------------------------------------------------------------
template <class T>
T
first (T a) {
    if (a)
        return a;

    throw std::logic_error ("no valid object");
}

template <class T, class ...Args>
T
first (T a, Args&& ...b) {
    if (a)
        return a;

    return first (std::forward<Args>(b)...);
}

#endif
