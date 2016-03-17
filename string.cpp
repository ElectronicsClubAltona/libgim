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
 * Copyright 2011-2016 Danny Robson <danny@nerdcruft.net>
 */

#include "./string.hpp"

#include <cstring>

using util::tokeniser;

///////////////////////////////////////////////////////////////////////////////
// TODO: Horribly inefficient, but God help you if you're relying on this
// being efficient in the first place.
bool
strbegins (const char *restrict str,
           const char *restrict prefix) {
    return 0 == strncmp (prefix, str, strlen (prefix));
}


///////////////////////////////////////////////////////////////////////////////
tokeniser::tokeniser (const string_type &_value,
                      value_type _separator):
    m_value (_value),
    m_separator (_separator)
{ }


//-----------------------------------------------------------------------------
tokeniser::iterator
tokeniser::cbegin (void) const
{
    return iterator (
        {m_value.cbegin (), m_value.cend ()},
        m_separator
    );
}


//-----------------------------------------------------------------------------
tokeniser::iterator
tokeniser::cend (void) const
{
    return iterator (
        {m_value.cend (), m_value.cend ()},
        m_separator
    );
}


///////////////////////////////////////////////////////////////////////////////
tokeniser::iterator::iterator (range_type _range, value_type _separator):
    m_separator (_separator),
    m_range (_range.cbegin (),
             std::find (_range.cbegin (),
                        _range.cend (),
                        _separator)),
    m_end (_range.cend ())
{ }


//-----------------------------------------------------------------------------
tokeniser::iterator
tokeniser::iterator::operator++ (int)
{
    iterator res(*this);
    ++*this;
    return res;
}


//-----------------------------------------------------------------------------
tokeniser::iterator&
tokeniser::iterator::operator++ (void)
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
tokeniser::iterator::range_type
tokeniser::iterator::operator* (void) const
{
    return m_range;
}


//-----------------------------------------------------------------------------
bool
tokeniser::iterator::operator== (const iterator &rhs) const
{
    return m_range == rhs.m_range && m_separator == rhs.m_separator;
}
