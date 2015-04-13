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

#include "guid.hpp"

#include "debug.hpp"
#include "endian.hpp"
#include "types.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;


guid::guid (uint32_t _data1,
            uint16_t _data2,
            uint16_t _data3,
            uint8_t  _data4[8]):
    data1 (_data1),
    data2 (_data2),
    data3 (_data3)
{
    copy (_data4, _data4 + elems (data4), data4);
}


guid::guid (void)
{
    ;
}


guid::guid (const char *str) {
    istringstream is (str);
    is >> *this;
}


guid::guid (const guid &rhs) {
    *this = rhs;
}


guid&
guid::operator= (const guid &rhs) {
    data1 = rhs.data1;
    data2 = rhs.data2;
    data3 = rhs.data3;
    copy (rhs.data4 + 0, rhs.data4 + elems (rhs.data4), data4 + 0);

    return *this;
}


guid
guid::from_bytes (const uint8_t *bytes) {
    guid g;

    g.data1 = *reinterpret_cast<const uint32_t *> (bytes);
    bytes += sizeof (g.data1);

    g.data2 = *reinterpret_cast<const uint16_t *> (bytes);
    bytes += sizeof (g.data2);

    g.data3 = *reinterpret_cast<const uint16_t *> (bytes);
    bytes += sizeof (g.data3);

    for (signed i = 7; i >= 0; --i)
        g.data4[i] = *bytes++;

    return g;
}


guid
guid::from_string (const char *bytes)
    { return guid (bytes); }


guid::type
guid::get_type (void) const {
    // Top three bits signal the type
    unsigned bits = data4[2] >> 5U;

    switch (bits) {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3: return NCS_BACKWARD;

        case 0x4:
        case 0x5: return STANDARD;

        case 0x6: return COM_BACKWARD;

        case 0x7: return RESERVED;
    }

    unreachable ();
}


bool
guid::operator< (const guid &rhs) const {
    if (data1 != rhs.data1)
        return data1 < rhs.data1;
    if (data2 != rhs.data2)
        return data2 < rhs.data2;
    if (data3 != rhs.data3)
        return data3 < rhs.data3;

    for (unsigned i = 0; i < elems (data4); ++i) {
        if (data4[i] == rhs.data4[i])
            return data4[i] < rhs.data4[i];
    }

    // All equal, so not less than
    return false;
}


bool
guid::operator== (const guid &rhs) const {
    return data1    == rhs.data1    &&
           data2    == rhs.data2    &&
           data3    == rhs.data3    &&
           data4[0] == rhs.data4[0] &&
           data4[1] == rhs.data4[1] &&
           data4[2] == rhs.data4[2] &&
           data4[3] == rhs.data4[3] &&
           data4[4] == rhs.data4[4] &&
           data4[5] == rhs.data4[5] &&
           data4[6] == rhs.data4[6] &&
           data4[7] == rhs.data4[7];
}

std::ostream&
operator<< (std::ostream &os, const guid &g) {
    uint64_t data4_b = static_cast<uint64_t> (g.data4[0]) <<  0u |
                       static_cast<uint64_t> (g.data4[1]) <<  8u |
                       static_cast<uint64_t> (g.data4[2]) << 16u |
                       static_cast<uint64_t> (g.data4[3]) << 24u |
                       static_cast<uint64_t> (g.data4[4]) << 32u |
                       static_cast<uint64_t> (g.data4[5]) << 40u;
    uint16_t data4_a = static_cast<uint16_t> (g.data4[6]) <<  0u |
                       static_cast<uint16_t> (g.data4[7]) <<  8u;

    os << hex << setfill ('0') << setw (2 * sizeof (g.data1)) << g.data1 << "-"
                               << setw (2 * sizeof (g.data2)) << g.data2 << "-"
                               << setw (2 * sizeof (g.data3)) << g.data3 << "-"
                               << setw (2 * 2) << data4_a << "-"
                               << setw (2 * 6) << data4_b;

    os << dec;
    return os;
}


std::istream&
operator>> (std::istream &is, guid &g) {
    bool braces = '{' == is.peek ();
    if (braces)
        is.get ();

    uint8_t dash1, dash2, dash3, dash4a;
    uint16_t data4_a;
    uint64_t data4_b;

    is >> hex >> g.data1 >> dash1
              >> g.data2 >> dash2
              >> g.data3 >> dash3
              >> data4_a >> dash4a
              >> data4_b;

    uint64_t data4 = data4_b | static_cast<uint64_t> (data4_a) << 48u;
    g.data4[0] = data4 & 0xFF; data4 >>= 8u;
    g.data4[1] = data4 & 0xFF; data4 >>= 8u;
    g.data4[2] = data4 & 0xFF; data4 >>= 8u;
    g.data4[3] = data4 & 0xFF; data4 >>= 8u;
    g.data4[4] = data4 & 0xFF; data4 >>= 8u;
    g.data4[5] = data4 & 0xFF; data4 >>= 8u;
    g.data4[6] = data4 & 0xFF; data4 >>= 8u;
    g.data4[7] = data4 & 0xFF;
        
    if (braces)
        is.get ();

    return is;
}

