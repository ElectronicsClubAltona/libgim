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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */


#include "view.hpp"

#include "debug.hpp"

#include <cstring>
#include <iterator>

//-----------------------------------------------------------------------------
util::view::view ():
    m_begin (nullptr),
    m_end   (nullptr)
{ ; }


//-----------------------------------------------------------------------------
util::view::view (const char *str):
    m_begin (str),
    m_end   (str + strlen (str))
{ ; }


//-----------------------------------------------------------------------------
util::view::view (const char *_begin,
                  const char *_end):
    m_begin (_begin),
    m_end   (_end)
{ ; }


//-----------------------------------------------------------------------------
const char*
util::view::begin (void) const
{
    return m_begin;
}


//-----------------------------------------------------------------------------
const char*
util::view::end (void) const
{
    return m_end;
}


//-----------------------------------------------------------------------------
bool
util::view::view::empty (void) const
{
    return m_begin == nullptr ||
           m_end   == nullptr ||
           m_begin == m_end;
}


//-----------------------------------------------------------------------------
size_t
util::view::size (void) const
{
    return m_end - m_begin;
}


//-----------------------------------------------------------------------------
const char&
util::view::operator[] (size_t idx) const
{
    CHECK_LT (m_begin + idx, m_end);
    return m_begin[idx];
}


//-----------------------------------------------------------------------------
bool
util::view::operator== (const char *restrict rhs) const
{
    return strlen (rhs) == size () &&
           0 == strncmp (rhs, m_begin, size ());
}


//-----------------------------------------------------------------------------
bool
util::view::operator== (view v) const
{
    return std::equal (m_begin, m_end, v.begin ());
}


//-----------------------------------------------------------------------------
std::ostream&
util::operator<< (std::ostream &os, util::view s)
{
    std::copy (s.begin (), s.end (), std::ostream_iterator<char> (os));
    return os;
}


//-----------------------------------------------------------------------------
bool
util::operator== (const char *str, view v)
{
    return v == str;
}
