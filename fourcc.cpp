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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#include "fourcc.hpp"

using util::fourcc;

static_assert (sizeof(fourcc) == 4, "fourcc must be a 4 byte POD");


///////////////////////////////////////////////////////////////////////////////
fourcc
fourcc::from_chars (uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    fourcc lhs;

    lhs.data[0] = a;
    lhs.data[1] = b;
    lhs.data[2] = c;
    lhs.data[3] = d;

    return lhs;
}


//-----------------------------------------------------------------------------
fourcc
fourcc::from_string (const char data[4]) {
    fourcc lhs;

    lhs.data[0] = static_cast<uint8_t> (data[0]);
    lhs.data[1] = static_cast<uint8_t> (data[1]);
    lhs.data[2] = static_cast<uint8_t> (data[2]);
    lhs.data[3] = static_cast<uint8_t> (data[3]);

    return lhs;
}


///////////////////////////////////////////////////////////////////////////////
bool
fourcc::operator== (const char rhs[4]) const {
    return data[0] == rhs[0] &&
           data[1] == rhs[1] &&
           data[2] == rhs[2] &&
           data[3] == rhs[3];
}


///////////////////////////////////////////////////////////////////////////////
fourcc::operator uint32_t (void) const {
    return static_cast<uint32_t> (data[0] << 24U |
                                  data[1] << 16U |
                                  data[2] <<  8U |
                                  data[3]);
}


///////////////////////////////////////////////////////////////////////////////
std::ostream&
util::operator<< (std::ostream &os, fourcc f) {
    os << f.data[0] << f.data[1] << f.data[2] << f.data[3];
    return os;
}
