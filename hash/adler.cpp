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
 * Copyright 2010-2014 Danny Robson <danny@nerdcruft.net>
 */

#include "adler.hpp"

#include "debug.hpp"
#include "fletcher.hpp"

static const unsigned MODULUS = 65521;

//-----------------------------------------------------------------------------
uint32_t
adler32 (const void* restrict _data, size_t _size)
{
    return adler32 (static_cast<const uint8_t*> (_data),
                    static_cast<const uint8_t*> (_data) + _size);
}


//-----------------------------------------------------------------------------
uint32_t
adler32 (const uint8_t *first, const uint8_t *last)
{
    return fletcher<32, MODULUS, 1, 0> (first, last - first);
}
