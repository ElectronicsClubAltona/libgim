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
 * Copyright 2014 Danny Robson <danny@nerdcruft.net>
 */

#include "rational.hpp"

#include <cstdint>

using util::rational;


//-----------------------------------------------------------------------------
template <typename T>
rational<T>::rational (T _n, T _d):
    n (_n),
    d (_d)
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
bool
rational<T>::operator== (const rational<T> &rhs) {
    return rhs.n == n && rhs.d == d;
}


//-----------------------------------------------------------------------------
template <typename T>
rational<T>::operator float (void) const {
    return static_cast<float> (n) / d;
}


//-----------------------------------------------------------------------------
template <typename T>
rational<T>::operator double (void) const {
    return static_cast<double> (n) / d;
}


//-----------------------------------------------------------------------------
template <typename T>
rational<T>::operator int (void) const {
    return n / d;
}


//-----------------------------------------------------------------------------
template <typename T>
rational<T>
rational<T>::inverse (void) const {
    return rational<T> { d, n };
}


//-----------------------------------------------------------------------------
template <typename T>
template <typename U>
rational<T>
rational<T>::operator* (const U &rhs) const {
    return { rhs * n, d };
}


//-----------------------------------------------------------------------------
template <typename T>
template <typename U>
rational<T>
rational<T>::operator/ (const U &rhs) const {
    return { n, rhs * d };
}


//-----------------------------------------------------------------------------
template <typename T, typename U>
rational<T>
util::operator/ (U lhs, rational<T> rhs) {
    return rhs.inverse () * lhs;
}

//-----------------------------------------------------------------------------
namespace util {
    template struct rational<uint32_t>;
    template struct rational<int32_t>;

    template rational<uint32_t> operator/ (int, rational<uint32_t>);
}
