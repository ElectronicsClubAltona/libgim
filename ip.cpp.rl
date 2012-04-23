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


#include "ip.hpp"

#include <stdexcept>
#include <iostream>


using namespace std;


const ipv4::ip ipv4::ip::LOOPBACK (127, 0, 0, 1);
const ipv4::ip ipv4::ip::ANY      (  0, 0, 0, 0);

const util::range<ipv4::port> ipv4::WELL_KNOWN_PORT (    0,  1023),
                              ipv4::REGISTERED_PORT ( 1024, 49151),
                              ipv4::PRIVATE_PORT    (49152, 65535);


ipv4::ip::ip (uint32_t _integer):
    m_integer (_integer)
{ ; }


ipv4::ip::ip (uint8_t a, uint8_t b, uint8_t c, uint8_t d):
    m_octets ({ a, b, c, d })
{ ; }


ipv4::ip&
ipv4::ip::operator = (const ipv4::ip &rhs) {
    m_octets[0] = rhs.m_octets[0];
    m_octets[1] = rhs.m_octets[1];
    m_octets[2] = rhs.m_octets[2];
    m_octets[3] = rhs.m_octets[3];

    return *this;
}


bool
ipv4::ip::operator == (const ipv4::ip &rhs) const {
    return m_octets[0] == rhs.m_octets[0] && 
           m_octets[1] == rhs.m_octets[1] &&
           m_octets[2] == rhs.m_octets[2] &&
           m_octets[3] == rhs.m_octets[3];
}


// RFC 3986
%%{
    machine ipv4;
    octet = (    [0-9][0-9]? |
              '1'[0-9][0-9]  |
              '2'[0-4][0-9]  |
             '25'[0-5])
            > {
                octetstart = fpc;
            }
            % {
                octetend   = fpc;
                __octet = 0;

                for (auto i = octetstart; i < octetend; ++i)
                    __octet = __octet * 10 + *i - '0';
            };

    ipv4 := (octet %{ __octets[0] = __octet; } '.'
             octet %{ __octets[1] = __octet; } '.'
             octet %{ __octets[2] = __octet; } '.'
             octet %{ __octets[3] = __octet; })
            > { __success = false; }
            % { __success =  true; }
            $!{ __success = false; };
}%%

%%write data;


ipv4::ip::ip (const std::string &data) {
    bool __success = true;
    uint8_t __octets[4] = { 0, 0, 0, 0 };
    const char *octetstart = nullptr,
               *octetend   = nullptr;
    uint8_t __octet;

    int cs = 0;
    const char *p   =     data.data (),
               *pe  = p + data.size (),
               *eof = pe;

    %%write init;
    %%write exec;

    if (!__success)
        throw invalid_argument(data);

    m_octets[0] = __octets[0];
    m_octets[1] = __octets[1];
    m_octets[2] = __octets[2];
    m_octets[3] = __octets[3];
}


ipv4::ip
ipv4::ip::parse (const string &data)
    { return ipv4::ip (data); }

