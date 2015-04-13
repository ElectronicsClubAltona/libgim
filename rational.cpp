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
