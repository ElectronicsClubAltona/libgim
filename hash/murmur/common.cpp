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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#include "common.hpp"

#include "../../debug.hpp"

///////////////////////////////////////////////////////////////////////////////
uint32_t
util::hash::murmur::tail (const uint32_t *restrict data, size_t len)
{
    auto bytes = reinterpret_cast<const uint8_t*> (data);
    uint32_t h = 0;

    switch (len % sizeof (uint32_t)) {
        case 3: h += bytes[2] << 16;
        case 2: h += bytes[1] <<  8;
        case 1: h += bytes[0];
                break;

        default:
            unreachable ();
    }

    return h;
}


//-----------------------------------------------------------------------------
uint64_t
util::hash::murmur::tail (const uint64_t *restrict data, size_t len)
{
    auto bytes = reinterpret_cast<const uint8_t*> (data);
    uint64_t h = 0;

    switch (len % sizeof (uint64_t)) {
        case 7: h += uint64_t(bytes[6]) << 48;
        case 6: h += uint64_t(bytes[5]) << 40;
        case 5: h += uint64_t(bytes[4]) << 32;
        case 4: h += uint64_t(bytes[3]) << 24;
        case 3: h += uint64_t(bytes[2]) << 16;
        case 2: h += uint64_t(bytes[1]) <<  8;
        case 1: h += uint64_t(bytes[0]);
                break;

        default:
            unreachable ();
    }

    return h;
}


//-----------------------------------------------------------------------------
std::array<uint32_t,4>
util::hash::murmur::tail_array (const uint32_t *restrict data, size_t len)
{
    auto bytes = reinterpret_cast<const uint8_t*> (data);
    std::array<uint32_t,4> result {0,0,0,0};

    switch (len % 16) {
        case 15: result[3] |= bytes[14] << 16;
        case 14: result[3] |= bytes[13] <<  8;
        case 13: result[3] |= bytes[12] <<  0;
        case 12: result[2] |= bytes[11] << 24;
        case 11: result[2] |= bytes[10] << 16;
        case 10: result[2] |= bytes[ 9] <<  8;
        case  9: result[2] |= bytes[ 8] <<  0;
        case  8: result[1] |= bytes[ 7] << 24;
        case  7: result[1] |= bytes[ 6] << 16;
        case  6: result[1] |= bytes[ 5] <<  8;
        case  5: result[1] |= bytes[ 4] <<  0;
        case  4: result[0] |= bytes[ 3] << 24;
        case  3: result[0] |= bytes[ 2] << 16;
        case  2: result[0] |= bytes[ 1] <<  8;
        case  1: result[0] |= bytes[ 0] <<  0;
                 break;

        default:
            unreachable ();
    }

    return result;
}


//-----------------------------------------------------------------------------
std::array<uint64_t,2>
util::hash::murmur::tail_array (const uint64_t *restrict data, size_t len)
{
    auto bytes = reinterpret_cast<const uint8_t*> (data);
    std::array<uint64_t,2> result {0, 0};

    switch(len & 15)
    {
        case 15: result[1] |= ((uint64_t)bytes[14]) << 48;
        case 14: result[1] |= ((uint64_t)bytes[13]) << 40;
        case 13: result[1] |= ((uint64_t)bytes[12]) << 32;
        case 12: result[1] |= ((uint64_t)bytes[11]) << 24;
        case 11: result[1] |= ((uint64_t)bytes[10]) << 16;
        case 10: result[1] |= ((uint64_t)bytes[ 9]) << 8;
        case  9: result[1] |= ((uint64_t)bytes[ 8]) << 0;

        case  8: result[0] |= ((uint64_t)bytes[ 7]) << 56;
        case  7: result[0] |= ((uint64_t)bytes[ 6]) << 48;
        case  6: result[0] |= ((uint64_t)bytes[ 5]) << 40;
        case  5: result[0] |= ((uint64_t)bytes[ 4]) << 32;
        case  4: result[0] |= ((uint64_t)bytes[ 3]) << 24;
        case  3: result[0] |= ((uint64_t)bytes[ 2]) << 16;
        case  2: result[0] |= ((uint64_t)bytes[ 1]) << 8;
        case  1: result[0] |= ((uint64_t)bytes[ 0]) << 0;
                 break;

        default:
            unreachable ();
    };

    return result;
}


