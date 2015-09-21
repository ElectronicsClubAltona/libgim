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


#include "view.hpp"

#include "debug.hpp"

#include <cstring>
#include <iterator>

using util::view;


//-----------------------------------------------------------------------------
template <typename T>
view<T>::view (T _begin, T _end):
    m_begin (_begin),
    m_end   (_end)
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
T
view<T>::begin (void)
{
    return m_begin;
}


//-----------------------------------------------------------------------------
template <typename T>
T
view<T>::end (void)
{
    return m_end;
}


//-----------------------------------------------------------------------------
template <typename T>
bool
view<T>::empty (void) const
{
    return m_begin == m_end;
}


//-----------------------------------------------------------------------------
template <typename T>
size_t
view<T>::size (void) const
{
    return std::distance (m_begin, m_end);
}


//-----------------------------------------------------------------------------
template <typename T>
const typename view<T>::value_type&
view<T>::operator[] (size_t idx) const
{
    CHECK_LT (m_begin + idx, m_end);
    return m_begin[idx];
}


//-----------------------------------------------------------------------------
template <typename T>
bool
view<T>::operator== (const view<T> rhs) const
{
    return rhs.m_begin == m_begin &&
           rhs.m_end   == m_end;
}


//-----------------------------------------------------------------------------
template <typename T>
std::ostream&
util::operator<< (std::ostream &os, util::view<T> v)
{
    std::copy (v.begin (), v.end (), std::ostream_iterator<char> (os));
    return os;
}


///////////////////////////////////////////////////////////////////////////////
template struct util::view<const char*>;
