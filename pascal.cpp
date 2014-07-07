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

#include "pascal.hpp"

#include <stdexcept>

using util::parray;


//-----------------------------------------------------------------------------
template <typename T>
parray<T>::parray (size_t _size, T *_data):
    size (_size),
    data (_data)
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
T&
parray<T>::operator[] (size_t idx) {
    return data[idx];
}


template <typename T>
const T&
parray<T>::operator[] (size_t idx) const {
    return data[idx];
}


//-----------------------------------------------------------------------------
template <typename T>
T&
parray<T>::at (size_t idx) {
    if (idx >= size)
        throw std::out_of_range ("invalid index for parray");

    return (*this)[idx];
}


template <typename T>
const T&
parray<T>::at (size_t idx) const {
    if (idx >= size)
        throw std::out_of_range ("invalid index for parray");

    return (*this)[idx];
}


//-----------------------------------------------------------------------------
template <typename T>
T*
parray<T>::begin (void) {
    return data;
}


template <typename T>
T*
parray<T>::end (void) {
    return data + size;
}


//-----------------------------------------------------------------------------
template <typename T>
const T*
parray<T>::cbegin (void) const {
    return data + size;
}


template <typename T>
const T*
parray<T>::cend (void) const {
    return data + size;
}


//-----------------------------------------------------------------------------
template <typename T>
std::ostream&
operator<< (std::ostream &os, parray<T> p) {
    os << "[" << p.size << ", " << std::hex << p.data << std::dec << "]";
    return os;
}


template std::ostream& operator<< (std::ostream&, parray<uint16_t>);
template std::ostream& operator<< (std::ostream&, parray<uint32_t>);
template std::ostream& operator<< (std::ostream&, parray<uint64_t>);

template std::ostream& operator<< (std::ostream&, parray<const uint16_t>);
template std::ostream& operator<< (std::ostream&, parray<const uint32_t>);
template std::ostream& operator<< (std::ostream&, parray<const uint64_t>);

//-----------------------------------------------------------------------------
namespace util {
    template struct parray<uint16_t>;
    template struct parray<uint32_t>;
    template struct parray<uint64_t>;

    template struct parray<const uint16_t>;
    template struct parray<const uint32_t>;
    template struct parray<const uint64_t>;
}
