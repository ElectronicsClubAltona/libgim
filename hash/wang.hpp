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
 * Copyright 2010-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_HASH_WANG_HPP
#define __UTIL_HASH_WANG_HPP

#include <cstdint>

namespace util::hash {
    // Thomas Wang's integer mixing functions, ca 2007
    //-------------------------------------------------------------------------
    constexpr uint32_t
    wang (uint32_t key)
    {
        // a prime or an odd constant
        constexpr uint32_t c2 = 0x27d4eb2d;

        key = (key ^ 61) ^ (key >> 16);
        key =  key       + (key <<  3);
        key =  key       ^ (key >>  4);
        key =  key       * c2;
        key =  key       ^ (key >> 15);

        return key;
    }


    //-------------------------------------------------------------------------
    constexpr uint64_t
    wang (uint64_t key)
    {
        key = ~key + (key << 21); // key = (key << 21) - key - 1;
        key =  key ^ (key >> 24);
        key = (key + (key <<  3)) + (key << 8); // key * 265
        key =  key ^ (key >> 14);
        key = (key + (key <<  2)) + (key << 4); // key * 21
        key =  key ^ (key >> 28);
        key =  key + (key << 31);

        return key;
    }
}

#endif
