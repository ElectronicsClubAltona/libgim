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
 * Copyright 2010 Danny Robson <danny@blubinc.net>
 */

#include "extent.hpp"

#include "debug.hpp"
#include "maths.hpp"

#include <cmath>

using namespace util;


template <typename T>
extent<T>::extent (const T  _width, const T  _height):
        width (_width),
        height (_height)
    { ; }


template <typename T>
T
extent<T>::diameter (void) const {
    return (T)sqrt (width * width + height * height);
}


template <typename T>
T
extent<T>::area (void) const
    { return width * height; }


template <typename T>
bool
extent<T>::empty (void) const
    { return almost_equal (area(), 0); }


template <typename T>
bool
extent<T>::operator ==(const extent& rhs) const {
    return almost_equal (width,  rhs.width) &&
           almost_equal (height, rhs.height);
}


template <typename T>
void
extent<T>::sanity (void) const
    { check (width >= 0 && height >= 0); }


namespace util {
    template <>
    void
    extent<unsigned int>::sanity (void) const
        { return; }

    template <>
    void
    extent<unsigned long>::sanity (void) const
        { return; }
}

template struct extent<unsigned int>;
template struct extent<unsigned long>;
template struct extent<double>;

