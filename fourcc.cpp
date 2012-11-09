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

#include "fourcc.hpp"

using util::fourcc;
using std::ostream;


static_assert (sizeof(fourcc) == 4, "fourcc must be a 4 byte POD");


fourcc
fourcc::from_chars (uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    fourcc lhs;

    lhs.data[0] = a;
    lhs.data[1] = b;
    lhs.data[2] = c;
    lhs.data[3] = d;

    return lhs;
}


fourcc
fourcc::from_string (const char data[4]) {
    fourcc lhs;

    lhs.data[0] = (uint8_t)data[0];
    lhs.data[1] = (uint8_t)data[1];
    lhs.data[2] = (uint8_t)data[2];
    lhs.data[3] = (uint8_t)data[3];

    return lhs;
}


bool
fourcc::operator== (const char rhs[4]) const {
    return data[0] == rhs[0] &&
           data[1] == rhs[1] &&
           data[2] == rhs[2] &&
           data[3] == rhs[3];
}


fourcc::operator uint32_t (void) const {
    return (uint32_t)(data[0] << 24U |
                      data[1] << 16U |
                      data[2] <<  8U |
                      data[3]);
}


ostream&
operator<< (ostream &os, fourcc f) {
    os << f.data[0] << f.data[1] << f.data[2] << f.data[3];
    return os;
}

