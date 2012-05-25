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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#include "string.hpp"

#define do_type_to_string(T)                                                    \
template <> std::string type_to_string <T>       (void) { return #T; }          \
template <> std::string type_to_string <const T> (void) { return "const " #T; }

do_type_to_string (float)
do_type_to_string (double)

do_type_to_string (  int8_t)
do_type_to_string ( int16_t)
do_type_to_string ( int32_t)
do_type_to_string ( int64_t)

do_type_to_string ( uint8_t)
do_type_to_string (uint16_t)
do_type_to_string (uint32_t)
do_type_to_string (uint64_t)

