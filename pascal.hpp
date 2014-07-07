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
 * Copyright 2010 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_PASCAL_HPP
#define __UTIL_PASCAL_HPP

#include <cstdlib>
#include <iostream>

namespace util {
    template <typename T>
    struct parray {
        parray (size_t size, T *data);

        size_t   size;
        T       *data;

        T& operator[] (size_t idx);
        const T& operator[] (size_t idx) const;

        T& at (size_t idx);
        const T& at (size_t idx) const;

        T* begin (void);
        T* end   (void);

        const T* cbegin (void) const;
        const T* cend   (void) const;
    };
}

template <typename T>
std::ostream& operator<< (std::ostream&, util::parray<T>);

#endif
