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

#include "view.hpp"

#include "debug.hpp"
#include "iterator.hpp"


///////////////////////////////////////////////////////////////////////////////
template <typename T>
constexpr
util::view<T>::view (T _begin, T _end) noexcept:
    m_begin (_begin),
    m_end   (_end)
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
template <typename K>
constexpr
util::view<T>::view (K &data):
    m_begin (std::begin (data)),
    m_end   (std::end   (data))
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
template <typename K>
constexpr
util::view<T>::view (const K &data):
    m_begin (std::begin (data)),
    m_end   (std::end (data))
{ ; }


///////////////////////////////////////////////////////////////////////////////
template <typename T>
constexpr T&
util::view<T>::begin (void) noexcept
{
    return m_begin;
}


//-----------------------------------------------------------------------------
template <typename T>
constexpr T&
util::view<T>::end (void) noexcept
{
    return m_end;
}


//-----------------------------------------------------------------------------
template <typename T>
constexpr const T&
util::view<T>::begin (void) const noexcept
{
    return cbegin ();
}


//-----------------------------------------------------------------------------
template <typename T>
constexpr const T&
util::view<T>::end (void) const noexcept
{
    return cend ();
}


//-----------------------------------------------------------------------------
template <typename T>
constexpr const T&
util::view<T>::cbegin (void) const noexcept
{
    return m_begin;
}


//-----------------------------------------------------------------------------
template <typename T>
constexpr const T&
util::view<T>::cend (void) const noexcept
{
    return m_end;   
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
constexpr T
util::view<T>::find (const value_type &v) const noexcept
{
   for (T i = cbegin (); i != cend (); ++i)
       if (*i == v)
           return i;
   return cend ();
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
constexpr
bool
util::view<T>::empty (void) const noexcept
{
    return m_begin == m_end;
}


//-----------------------------------------------------------------------------
template <typename T>
constexpr
size_t
util::view<T>::size (void) const noexcept
{
    return m_end - m_begin;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
constexpr
const typename util::view<T>::value_type&
util::view<T>::operator[] (size_t idx) const noexcept
{
    return m_begin[idx];
}


//-----------------------------------------------------------------------------
template <typename T>
constexpr
typename util::view<T>::value_type&
util::view<T>::operator[] (size_t idx) noexcept
{
    return m_begin[idx];
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
bool
util::view<T>::operator== (const view<T> rhs) const noexcept
{
    return rhs.m_begin == m_begin &&
           rhs.m_end   == m_end;
}


//-----------------------------------------------------------------------------
template <typename T>
bool
util::operator!= (const view<T> a, const view<T> b)
{
    return !(a == b);
}


///////////////////////////////////////////////////////////////////////////////
template <typename T, size_t N>
auto
util::make_view (const T (&arr)[N])
{
    return util::view<const T*> (arr + 0, arr + N);
}


//-----------------------------------------------------------------------------
template <typename T>
auto
util::make_view (T &t)
{
    return util::view<decltype(std::begin (t))> { std::begin (t), std::end (t) };
}


//-----------------------------------------------------------------------------
template <typename T>
auto
util::make_view (const T &t)
{
    return util::view<decltype(std::cbegin (t))> { std::cbegin (t), std::cend (t) };
}


//-----------------------------------------------------------------------------
template <typename T>
auto
util::make_cview (const T &t)
{
    return util::view<decltype(std::cbegin (t))> { std::cbegin (t), std::cend (t) };
}


///////////////////////////////////////////////////////////////////////////////
inline
util::view<const char*>
util::make_view (const char *str)
{
    return { str, str + strlen (str) };
}

//-----------------------------------------------------------------------------
inline
util::view<char*>
util::make_view (char *str)
{
    return { str, str + strlen (str) };
}


//-----------------------------------------------------------------------------
template <typename CharT, typename TraitsT, typename AllocT>
util::view<const CharT*>
util::make_view (const std::basic_string<CharT,TraitsT,AllocT> &str)
{
    return {
        std::data (str),
        std::data (str) + std::size (str)
    };
}


//-----------------------------------------------------------------------------
template <typename CharT, typename TraitsT, typename AllocT>
util::view<CharT*>
util::make_view (std::basic_string<CharT,TraitsT,AllocT> &str)
{
    return {
        std::data (str),
        std::data (str) + std::size (str)
    };
}
