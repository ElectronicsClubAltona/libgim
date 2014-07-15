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
 * Copyright 2010 Danny Robson <danny@nerdcruft.net>
 */

#include "stream.hpp"

#include "debug.hpp"

using util::stream::null;
using util::stream::bits;

//-----------------------------------------------------------------------------
std::ostream&
null::put (char)
    { return *this; }


//-----------------------------------------------------------------------------
bool
null::good (void) const
    { return !bad () && !eof () && !fail (); }


//-----------------------------------------------------------------------------
bool null::bad  (void) const { return false; }
bool null::eof  (void) const { return false; }
bool null::fail (void) const { return false; }


//-----------------------------------------------------------------------------
bits::bits (uintmax_t _value, unsigned _count):
    value (_value),
    count (_count)
{
    CHECK_LE (count, sizeof(value) * 8);
}


//-----------------------------------------------------------------------------
std::ostream&
operator<< (std::ostream &os, util::stream::bits b) {
    char digits[sizeof (b.value) * 8 + 1] = { 0 };
    char *cursor = std::end (digits) - 1;

    uintmax_t bits  = b.value;
    unsigned  count = b.count;

    while (count) {
        CHECK_GE (cursor, digits);

        --cursor;
        *cursor = (bits & 0x01) ? '1' : '0';

        bits >>= 1;
        count -= 1;
    }

    os << cursor << "/" << b.count;
    return os;
}
