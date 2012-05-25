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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __TYPES_HPP
#define __TYPES_HPP

#include <cstdint>
#include <cstdlib>
#include <memory>

/// Returns the number of elements of a statically allocated array
template <typename T, size_t N>  
size_t elems(T (&)[N])
    { return N; }


template<class T, class...Args>
std::unique_ptr<T>
make_unique(Args&&... args) {
    return std::unique_ptr<T> (new T(std::forward<Args>(args)...));
}


#endif // __TYPES_HPP
