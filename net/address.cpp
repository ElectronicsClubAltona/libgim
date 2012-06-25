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

#include "address.hpp"

#include "except.hpp"
#include "../debug.hpp"
#include "../endian.hpp"
#include "../types.hpp"

#ifdef __WIN32
#else
    #include <arpa/inet.h>
#endif

//-----------------------------------------------------------------------------
using namespace net;
using namespace std;


//-----------------------------------------------------------------------------
#ifdef __WIN32
const char* inet_ntop(int af, const void* src, char* dst, int size){
    struct sockaddr_in srcaddr;
    memset(&srcaddr, 0, sizeof(struct sockaddr_in));
    memcpy(&(srcaddr.sin_addr), src, sizeof(srcaddr.sin_addr));
    srcaddr.sin_family = af;

    if (WSAAddressToString ((struct sockaddr*) &srcaddr, sizeof (struct sockaddr_in), 0, dst, (LPDWORD) &size) != 0)
        net::error::throw_code ();

    return dst;
}
#endif


//-----------------------------------------------------------------------------
namespace net {
    template <>
    address<domain::INET>::address (const sockaddr_type &addr):
        m_ip   (addr.sin_addr.s_addr),
        m_mask (0),
        m_port (ntoh (addr.sin_port))
    {
        CHECK (addr.sin_family == (int)domain::INET);
    }


    template <domain D>
    address<D>::address (const std::string &_ip,
                         port_type          _port):
        m_ip   (_ip),
        m_mask (  0),
        m_port (_port)
    { ; }


    template <>
    address<domain::INET>::sockaddr_type
    address<domain::INET>::to_sockaddr (void) const {
        sockaddr_type addr;

        addr.sin_family      = (int)domain::INET;
        addr.sin_port        = hton (m_port);
        addr.sin_addr.s_addr = m_ip.m_integer;

        return addr;
    }


    template <>
    std::string
    address<domain::INET>::to_string (void) const {
        char dest[INET_ADDRSTRLEN + 1];
        sockaddr_type addr = to_sockaddr ();

        if (NULL == inet_ntop ((int)domain::INET, &addr.sin_addr, dest, sizeof (dest)))
            net::error::throw_code ();
        return dest;
    }


    template <>
    bool
    address<domain::INET>::operator ==(const address<domain::INET> &rhs) {
        return m_ip   == rhs.m_ip   &&
               m_mask == rhs.m_mask &&
               m_port == rhs.m_port;
    }
}


//-----------------------------------------------------------------------------
std::ostream&
net::operator<< (std::ostream &os, const address<net::domain::INET> &addr) {
    os << addr.to_string () << ":" << addr.port ();
    return os;
}


//-----------------------------------------------------------------------------
template <>
const address<domain::INET>
address<domain::INET>::LOOPBACK  ("127.0.0.1", 0);


template <>
const address<domain::INET>
address<domain::INET>::ANY       ("0.0.0.0", 0);


template <>
const address<domain::INET6>
address<domain::INET6>::LOOPBACK ("::1", 0);


template <>
const address<domain::INET6>
address<domain::INET6>::ANY      ("::0", 0);

template class address<domain::INET>;
template class address<domain::INET6>;

