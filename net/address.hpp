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

#ifndef __NET_ADDRESS_HPP
#define __NET_ADDRESS_HPP

#include "types.hpp"
#include "../ip.hpp"

#if defined(HAVE_WINSOCK2_H)
	#include "ws2tcpip.h"
#endif

#include <iostream>
#include <string>


//-----------------------------------------------------------------------------
namespace net {
    /// Supporting types used for defining addresses in various domains
    template <domain D>
    struct address_types;


    template <>
    struct address_types <domain::INET> {
        typedef ipv4::ip    ip;
        typedef ipv4::mask  mask;
        typedef ipv4::port  port;
        typedef sockaddr_in sockaddr;
    };


    template <>
    struct address_types <domain::INET6> {
        typedef ipv6::ip     ip;
        typedef ipv6::mask   mask;
        typedef ipv6::port   port;
        typedef sockaddr_in6 sockaddr;
    };


    /// A full endpoint specification for a domain. Must be usable for bind/listen and send/recv.
    template <domain D>
    class address {
        public:
            typedef typename address_types<D>::ip       ip_type;
            typedef typename address_types<D>::mask     mask_type;
            typedef typename address_types<D>::port     port_type;
            typedef typename address_types<D>::sockaddr sockaddr_type;

        protected:
            ip_type   m_ip;
            mask_type m_mask;
            port_type m_port;

        public:
            static const address<D> LOOPBACK;
            static const address<D> ANY;

            address (const sockaddr_type &);
            address (const std::string &address, port_type);

            port_type port (void) const;
            void set_port (const port_type &);

            ip_type ip (void) const         { return m_ip; }

            sockaddr_type to_sockaddr (void) const;
            std::string   to_string   (void) const;

            static ip_type resolve (const std::string &);

            bool operator ==(const address<D> &rhs);
    };

    std::ostream&
    operator<< (std::ostream &os, const net::address<net::domain::INET> &addr);
}



#endif // __NET_ADDRESS_HPP

