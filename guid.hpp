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
 * Copyright 2011 Danny Robson <danny@blubinc.net>
 */

#ifndef __UTIL_GUID_HPP
#define __UTIL_GUID_HPP

#include <cstdint>
#include <iostream>

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
        guid (const char *);
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
