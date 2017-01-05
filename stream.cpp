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
 * Copyright 2010 Danny Robson <danny@nerdcruft.net>
 */

#include "stream.hpp"

#include "debug.hpp"

using util::stream::null_ostream;
using util::stream::bits;

//-----------------------------------------------------------------------------
std::ostream&
null_ostream::put (char)
{
    return *this;
}


//-----------------------------------------------------------------------------
bool
null_ostream::good (void) const
{
    return !bad () && !eof () && !fail ();
}


//-----------------------------------------------------------------------------
bool null_ostream::bad  (void) const { return false; }
bool null_ostream::eof  (void) const { return false; }
bool null_ostream::fail (void) const { return false; }


///////////////////////////////////////////////////////////////////////////////
bits::bits (uintmax_t _value, unsigned _count):
    value (_value),
    count (_count)
{
    CHECK_LE (count, sizeof(value) * 8);
}


//-----------------------------------------------------------------------------
std::ostream&
util::stream::operator<< (std::ostream &os, util::stream::bits b) {
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
