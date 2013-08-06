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

#ifndef __UTIL_RANDOM_HPP
#define __UTIL_RANDOM_HPP

#include <iterator>

namespace util {
    template <typename T>
    T& randomise (T &);

    template <typename T, size_t N>
    T* randomise (T(&)[N]);

    template <typename T>
    T random (void);

    template <typename T>
    typename T::value_type&
    choose (T &container) {
        typename T::iterator  cursor = container.begin ();
        typename T::size_type size   = container.size ();
        typename T::size_type offset = random<typename T::size_type> () % size;
        
        std::advance (cursor, offset);
        return *cursor;
    }

    template <typename T, size_t N>
    T&
    choose (T (&v)[N]) {
        return v[static_cast<size_t> (random<float> () * N)];
    }

    template <typename T>
    typename T::value_type&
    choose (T begin, T end) {
        typename T::difference_type size = std::distance (begin, end);
        std::advance (begin, random<T::size_type> () % size);
        return *begin;
    }
}

#include "random.ipp"

#endif
