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
 * Copyright 2011-2017 Danny Robson <danny@nerdcruft.net>
 */

#include "./string.hpp"

#include "./debug.hpp"

#include <cstring>
#include <codecvt>
#include <locale>

using util::tokeniser;


///////////////////////////////////////////////////////////////////////////////
std::string
util::to_utf8 (const wchar_t *src)
{
    using convert_t = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_t,wchar_t> converter;
    return converter.to_bytes (src);
}


//-----------------------------------------------------------------------------
std::string
util::to_utf8 (const std::wstring &src)
{
    return to_utf8 (src.c_str ());
}


///////////////////////////////////////////////////////////////////////////////
// TODO: Horribly inefficient, but God help you if you're relying on this
// being efficient in the first place.
bool
strbegins (const char *restrict str,
           const char *restrict prefix) {
    return 0 == strncmp (prefix, str, strlen (prefix));
}


///////////////////////////////////////////////////////////////////////////////
template <typename Iterator>
tokeniser<Iterator>::tokeniser (Iterator _first,
                                Iterator _last,
                                value_type _separator):
    m_range (_first, _last),
    m_separator (_separator)
{ }


///////////////////////////////////////////////////////////////////////////////
template <typename Iterator>
typename tokeniser<Iterator>::iterator
tokeniser<Iterator>::cbegin (void) const
{
    return iterator (
        m_range,
        m_separator
    );
}


//-----------------------------------------------------------------------------
template <typename Iterator>
typename tokeniser<Iterator>::iterator
tokeniser<Iterator>::cend (void) const
{
    return iterator (
        {m_range.cend (), m_range.cend ()},
        m_separator
    );
}


//-----------------------------------------------------------------------------
template <typename Iterator>
typename tokeniser<Iterator>::iterator
tokeniser<Iterator>::begin (void) const
{
    return cbegin ();
}


//-----------------------------------------------------------------------------
template <typename Iterator>
typename tokeniser<Iterator>::iterator
tokeniser<Iterator>::end (void) const
{
    return cend ();
}


///////////////////////////////////////////////////////////////////////////////
template <typename Iterator>
tokeniser<Iterator>::iterator::iterator (range_type _range, value_type _separator):
    m_separator (_separator),
    m_range (_range.cbegin (),
             std::find (_range.cbegin (),
                        _range.cend (),
                        _separator)),
    m_end (_range.cend ())
{ }


//-----------------------------------------------------------------------------
template <typename Iterator>
typename tokeniser<Iterator>::iterator
tokeniser<Iterator>::iterator::operator++ (int)
{
    iterator res(*this);
    ++*this;
    return res;
}


//-----------------------------------------------------------------------------
template <typename Iterator>
typename tokeniser<Iterator>::iterator&
tokeniser<Iterator>::iterator::operator++ (void)&
{
    auto newend = m_range.cend ();
    if (newend != m_end) {
        CHECK_EQ (*m_range.cend (), m_separator);
        newend++;
    }

    m_range = {
        newend,
        std::find (newend, m_end, m_separator)
    };

    return *this;
}


//-----------------------------------------------------------------------------
template <typename Iterator>
typename tokeniser<Iterator>::range_type
tokeniser<Iterator>::iterator::operator* (void) const
{
    return m_range;
}


//-----------------------------------------------------------------------------
template <typename Iterator>
bool
tokeniser<Iterator>::iterator::operator== (const iterator &rhs) const
{
    return m_range == rhs.m_range && m_separator == rhs.m_separator;
}


//-----------------------------------------------------------------------------
template <typename Iterator>
bool
tokeniser<Iterator>::iterator::operator!= (const iterator &rhs) const
{
    return !(*this == rhs);
}


///////////////////////////////////////////////////////////////////////////////
tokeniser<std::string::const_iterator>
util::make_tokeniser (const std::string &value, std::string::value_type separator)
{
    return tokeniser<std::string::const_iterator> (value.cbegin (), value.cend (), separator);
}


//-----------------------------------------------------------------------------
tokeniser<const char*>
util::make_tokeniser (const char *str, char separator)
{
    return tokeniser<const char*> (str, str + strlen (str), separator);
}


///////////////////////////////////////////////////////////////////////////////
template struct util::tokeniser<std::string::const_iterator>;
template struct util::tokeniser<const char*>;
