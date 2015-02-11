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
    begin (nullptr),
    end   (nullptr)
{ ; }


//-----------------------------------------------------------------------------
util::view::view (const char *str):
    begin (str),
    end   (str + strlen (str))
{ ; }


//-----------------------------------------------------------------------------
bool
util::view::view::empty (void) const
{
    return begin == nullptr ||
           end   == nullptr ||
           begin == end;
}


//-----------------------------------------------------------------------------
size_t
util::view::size (void) const
{
    return end - begin;
}


//-----------------------------------------------------------------------------
const char&
util::view::operator[] (size_t idx) const
{
    CHECK_LT (begin + idx, end);
    return begin[idx];
}


//-----------------------------------------------------------------------------
std::ostream&
util::operator<< (std::ostream &os, util::view s)
{
    std::copy (s.begin, s.end, std::ostream_iterator<char> (os));
    return os;
}

