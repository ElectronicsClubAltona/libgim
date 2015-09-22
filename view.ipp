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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */


#ifdef __UTIL_VIEW_IPP
#error
#endif
#define __UTIL_VIEW_IPP


#include "debug.hpp"

#include <iterator>

//-----------------------------------------------------------------------------
template <typename T>
util::view<T>::view (T _begin, T _end):
    m_begin (_begin),
    m_end   (_end)
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
T
util::view<T>::begin (void)
{
    return m_begin;
}


//-----------------------------------------------------------------------------
template <typename T>
T
util::view<T>::end (void)
{
    return m_end;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
bool
util::view<T>::empty (void) const
{
    return m_begin == m_end;
}


//-----------------------------------------------------------------------------
template <typename T>
size_t
util::view<T>::size (void) const
{
    return std::distance (m_begin, m_end);
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
const typename util::view<T>::value_type&
util::view<T>::operator[] (size_t idx) const
{
    CHECK_LT (m_begin + idx, m_end);
    return m_begin[idx];
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
bool
util::view<T>::operator== (const view<T> rhs) const
{
    return rhs.m_begin == m_begin &&
           rhs.m_end   == m_end;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
std::ostream&
util::operator<< (std::ostream &os, util::view<T> v)
{
    std::copy (v.begin (), v.end (), std::ostream_iterator<char> (os));
    return os;
}
