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

#include "maths.hpp"

#include <cstdint>

using util::rational;


///////////////////////////////////////////////////////////////////////////////
template <typename T>
rational<T>::rational (T _n, T _d):
    n (_n),
    d (_d)
{
    if (n < 0 && d < 0) {
        n *= -1;
        d *= -1;
    }
}


//-----------------------------------------------------------------------------
template <typename T>
rational<T>::rational (T v):
    n (v),
    d (1)
{ ; }


///////////////////////////////////////////////////////////////////////////////
template <typename T>
bool
rational<T>::operator== (const rational<T> rhs) const
{
    return rhs.n == n && rhs.d == d;
}


//-----------------------------------------------------------------------------
template <typename T>
bool
rational<T>::operator!= (const rational<T> rhs) const
{
    return !operator== (rhs);
}


//-----------------------------------------------------------------------------
template <typename T>
bool
rational<T>::operator< (const rational<T> rhs) const
{
    return n * rhs.d < rhs.n * d;
}


//-----------------------------------------------------------------------------
template <typename T>
bool
rational<T>::operator>= (const rational<T> rhs) const
{
    return n * rhs.d >= rhs.n * d;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
rational<T>::operator float (void) const
{
    return static_cast<float> (n) / d;
}


//-----------------------------------------------------------------------------
template <typename T>
rational<T>::operator double (void) const
{
    return static_cast<double> (n) / d;
}


//-----------------------------------------------------------------------------
template <typename T>
rational<T>::operator int (void) const
{
    return static_cast<int> (n / d);
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
rational<T>
rational<T>::reduced (void) const
{
    auto x = gcd (abs (n), abs (d));

    return { n / x, d / x };
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
rational<T>
rational<T>::inverse (void) const
{
    return rational<T> { d, n };
}


//-----------------------------------------------------------------------------
template <typename T>
rational<T>
rational<T>::operator+ (const T rhs) const
{
    return { n + rhs * d, d };
}


//-----------------------------------------------------------------------------
template <typename T>
rational<T>
rational<T>::operator- (const T rhs) const
{
    return { n - rhs * d, d };
}


//-----------------------------------------------------------------------------
template <typename T>
rational<T>
rational<T>::operator* (const T rhs) const
{
    return { rhs * n, d };
}


//-----------------------------------------------------------------------------
template <typename T>
rational<T>
rational<T>::operator/ (const T rhs) const
{
    return { n, rhs * d };
}


//-----------------------------------------------------------------------------
template <typename T>
rational<T>
util::operator/ (T lhs, rational<T> rhs) {
    return rhs.inverse () * lhs;
}

//-----------------------------------------------------------------------------
template struct util::rational<uint32_t>;
template struct util::rational< int32_t>;

template util::rational<uint32_t> util::operator/ (uint32_t, util::rational<uint32_t>);
