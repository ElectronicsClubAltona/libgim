
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
 
#include "except.hpp"


//-----------------------------------------------------------------------------
json::parse_error::parse_error (const std::string &_what, size_t _line):
    error (_what),
    line  (_line)
{ ; }


//-----------------------------------------------------------------------------
json::key_error::key_error (std::string _what):
    error (std::move (_what))
{ ; }
