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

#ifndef __UTIL_GUID_HPP
#define __UTIL_GUID_HPP

#include <cstdint>
#include <ostream>
#include <istream>

class guid {
    public:
        enum type {
            NCS_BACKWARD, // 0xx: Network Computing System backward compatibility
            STANDARD,     // 10x: Standard
            COM_BACKWARD, // 110: Microsoft Component Object Model backward compatibility
            RESERVED      // 111: Reserved for future use
        };

    private:
        guid (void);

    public:
        guid (uint32_t, uint16_t, uint16_t, uint8_t[8]);
        explicit guid (const char *);
        guid (const guid&);

        guid& operator= (const guid&);

        static guid from_bytes  (const uint8_t *bytes);
        static guid from_string (const char    *bytes);

        uint32_t data1;
        uint16_t data2;
        uint16_t data3;
        uint8_t  data4[8];

        type get_type (void) const;

        bool operator<  (const guid&) const;
        bool operator== (const guid&) const;
};

std::istream& operator>> (std::istream&,       guid&);
std::ostream& operator<< (std::ostream&, const guid&);

#endif
