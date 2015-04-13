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

#ifndef __UTIL_FOURCC_HPP
#define __UTIL_FOURCC_HPP

#include <iostream>
#include <cstdint>

namespace util {
    struct fourcc {
        uint8_t data[4];

        static fourcc from_string(const char[4]);
        static fourcc from_chars(uint8_t, uint8_t, uint8_t, uint8_t);

        bool operator== (const char[4]) const;
        operator uint32_t (void) const;
    };
}

std::ostream& operator<< (std::ostream&, util::fourcc);

#endif
