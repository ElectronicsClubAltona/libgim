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
 * Copyright 2011-2015 Danny Robson <danny@nerdcruft.net>
 */


#ifdef __UTIL_MATRIX_IPP
#error
#endif

#define __UTIL_MATRIX_IPP


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
T*
util::matrix<S,T>::operator[] (size_t idx)
{
    return this->values[idx];
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
const T*
util::matrix<S,T>::operator[] (size_t idx) const
{
    return this->values[idx];
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
const T*
util::matrix<S,T>::begin (void) const
{
    return &(*this)[0][0];
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
const T*
util::matrix<S,T>::end (void) const
{
    return &(*this)[S][0];
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
const T*
util::matrix<S,T>::cbegin (void) const
{
    return begin ();
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
const T*
util::matrix<S,T>::cend (void) const
{
    return end ();
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
T*
util::matrix<S,T>::begin (void)
{
    return &(*this)[0][0];
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
T*
util::matrix<S,T>::end (void)
{
    return &(*this)[S][0];
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
template <typename U>
util::matrix<S,U>
util::matrix<S,T>::cast (void) const
{
    util::matrix<S,U> out;
    std::copy (cbegin (), cend (), std::begin (out));
    return out;
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
util::matrix<S,T>
util::abs (const util::matrix<S,T> &src)
{
    util::matrix<S,T> dst;
    std::transform (std::cbegin (src), std::cend (src), std::begin (dst), util::abs<T>);
    return dst;
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
constexpr
T
util::sum (const util::matrix<S,T> &src)
{
    return sum (std::cbegin (src), std::cend (src));
}
