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

#ifndef __UTIL_IP_HPP
#define __UTIL_IP_HPP

#include "range.hpp"

#include <cstdint>
#include <string>


namespace ipv4 {
    struct ip {
        union {
            uint8_t  m_octets[4];
            uint32_t m_integer;
        };

        ip (const std::string &);
        ip (uint32_t i);
        ip (uint8_t a, uint8_t b, uint8_t c, uint8_t d);

        ip&  operator  = (const ip &);
        bool operator == (const ip &) const;

        explicit operator uint32_t (void);

        static ip parse (const std::string &);

        static const ip LOOPBACK;
        static const ip ANY;
    };

    
    typedef uint16_t port;
    typedef uint32_t mask;

    extern const util::range<port> WELL_KNOWN_PORT,
                             REGISTERED_PORT,
                             PRIVATE_PORT;
}


namespace ipv6 {
    struct ip {
        uint32_t m_quads[4];

        ip (const std::string&) { ; }
    };

    typedef uint16_t port;

    struct mask {
        uint32_t m_quads[4];

        mask (uint32_t) { ; }
    };
}


#endif // __UTIL_IP_HPP
