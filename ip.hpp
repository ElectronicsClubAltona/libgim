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

#ifndef __UTIL_IP_HPP
#define __UTIL_IP_HPP

#include <cstdint>
#include <string>


namespace ipv4 {
    struct ip {
        uint8_t m_octets[4];

        ip (uint8_t a, uint8_t b, uint8_t c, uint8_t d);

        ip&  operator  = (const ip &);
        bool operator == (const ip &) const; 

        static ip parse (const std::string &);
    };

    
    typedef uint16_t port_t;
}


#endif // __UTIL_IP_HPP
