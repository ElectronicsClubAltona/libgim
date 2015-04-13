/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
parray<T>::begin (void) const {
    return data;
}


template <typename T>
T*
parray<T>::end (void) const {
    return data + size;
}


//-----------------------------------------------------------------------------
template <typename T>
const T*
parray<T>::cbegin (void) const {
    return data;
}


template <typename T>
const T*
parray<T>::cend (void) const {
    return data + size;
}


//-----------------------------------------------------------------------------
template <typename T>
std::ostream&
util::operator<< (std::ostream &os, parray<T> p) {
    os << "[" << p.size << ", " << std::hex << p.data << std::dec << "]";
    return os;
}


template std::ostream& util::operator<< (std::ostream&, parray<uint16_t>);
template std::ostream& util::operator<< (std::ostream&, parray<uint32_t>);
template std::ostream& util::operator<< (std::ostream&, parray<uint64_t>);

template std::ostream& util::operator<< (std::ostream&, parray<const uint16_t>);
template std::ostream& util::operator<< (std::ostream&, parray<const uint32_t>);
template std::ostream& util::operator<< (std::ostream&, parray<const uint64_t>);

//-----------------------------------------------------------------------------
namespace util {
    template struct parray<char>;
    template struct parray<const char>;

    template struct parray<int8_t>;
    template struct parray<int16_t>;
    template struct parray<int32_t>;
    template struct parray<int64_t>;

    template struct parray<uint8_t>;
    template struct parray<uint16_t>;
    template struct parray<uint32_t>;
    template struct parray<uint64_t>;

    template struct parray<const int8_t>;
    template struct parray<const int16_t>;
    template struct parray<const int32_t>;
    template struct parray<const int64_t>;

    template struct parray<const uint8_t>;
    template struct parray<const uint16_t>;
    template struct parray<const uint32_t>;
    template struct parray<const uint64_t>;
}
