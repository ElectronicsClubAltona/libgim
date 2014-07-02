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
 * Copyright 2010-2012 Danny Robson <danny@nerdcruft.net>
 */

#include "extent.hpp"

#include "debug.hpp"
#include "maths.hpp"

#include <cmath>

using namespace util;


//-----------------------------------------------------------------------------
template <typename T>
extent<T>::extent (const T  _width, const T  _height):
        w (_width),
        h (_height)
{ ; }


template <typename T>
extent<T>::extent (const extent<T> &rhs):
        w (rhs.w),
        h (rhs.h)
{ ; }


template <typename T>
extent<T>&
extent<T>::operator= (const extent<T> &rhs) {
    w = rhs.w;
    h = rhs.h;

    return *this;
}


//-----------------------------------------------------------------------------
template <typename T>
T
extent<T>::diameter (void) const {
    return static_cast<T> (sqrt (w * w + h * h));
}


template <typename T>
T
extent<T>::area (void) const
    { return w * h; }


//-----------------------------------------------------------------------------
template <typename T>
double
extent<T>::aspect (void) const
    { return static_cast<double> (w) / h; }


//-----------------------------------------------------------------------------
template <typename T>
bool
extent<T>::empty (void) const
    { return almost_equal (area(), 0); }


//-----------------------------------------------------------------------------
template <typename T>
bool
extent<T>::operator ==(const extent& rhs) const {
    return almost_equal (w, rhs.w) &&
           almost_equal (h, rhs.h);
}


//-----------------------------------------------------------------------------
template <typename T>
void
extent<T>::sanity (void) const
    { CHECK (w >= 0 && h >= 0); }


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

//-----------------------------------------------------------------------------
template <typename T>
std::ostream&
operator<< (std::ostream &os, util::extent<T> e) {
    os << "[" << e.w << ", " << e.h << "]";
    return os;
}


template std::ostream& operator<< (std::ostream&, util::extent<uint16_t>);
template std::ostream& operator<< (std::ostream&, util::extent<uint32_t>);
template std::ostream& operator<< (std::ostream&, util::extent<uint64_t>);

//-----------------------------------------------------------------------------
namespace util {
    template struct extent<uint32_t>;
    template struct extent<uint64_t>;
    template struct extent<float>;
    template struct extent<double>;
}
