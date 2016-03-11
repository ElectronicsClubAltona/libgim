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

#include "address.hpp"

#include "except.hpp"
#include "../debug.hpp"
#include "../endian.hpp"
#include "../types.hpp"
#include "../raii.hpp"


#ifdef __WIN32
#else
    #include <netdb.h>
    #include <arpa/inet.h>
#endif

using net::address;


///////////////////////////////////////////////////////////////////////////////
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


///////////////////////////////////////////////////////////////////////////////
template <net::domain D>
typename address<D>::port_type
address<D>::port (void) const
    { return m_port; }


//-----------------------------------------------------------------------------
template <net::domain D>
void
address<D>::set_port (const port_type &_port)
    { m_port = _port; }


//-----------------------------------------------------------------------------
template <net::domain D>
typename address<D>::ip_type
address<D>::resolve (const std::string &str) {
    addrinfo  hint;
    memset (&hint, 0, sizeof (hint));
    hint.ai_family = static_cast<int> (D);

    addrinfo* resolved;
    int err = getaddrinfo (str.c_str (), nullptr, nullptr, &resolved);
    if (err)
        net::error::throw_code ();

    auto deletor = [] (addrinfo *a) { freeaddrinfo (a); };
    std::unique_ptr<addrinfo, decltype(deletor)> raii(resolved, deletor);
    return ip_type (reinterpret_cast<sockaddr_type*> (resolved->ai_addr)->sin_addr.s_addr);
}


///////////////////////////////////////////////////////////////////////////////
namespace net {
    template <>
    address<net::domain::INET>::address (const sockaddr_type &addr):
        m_ip   (addr.sin_addr.s_addr),
        m_mask (0),
        m_port (ntoh (addr.sin_port))
    {
        CHECK (addr.sin_family == (int)net::domain::INET);
    }


    template <net::domain D>
    address<D>::address (const std::string &_addr,
                         port_type          _port):
        m_ip   (resolve (_addr)),
        m_mask (  0),
        m_port (_port)
    { ; }


    template <>
    address<net::domain::INET>::sockaddr_type
    address<net::domain::INET>::to_sockaddr (void) const {
        sockaddr_type addr;

        addr.sin_family      = (int)net::domain::INET;
        addr.sin_port        = hton (m_port);
        addr.sin_addr.s_addr = m_ip.m_integer;

        return addr;
    }


    template <>
    std::string
    address<net::domain::INET>::to_string (void) const {
        char dest[INET_ADDRSTRLEN + 1];
        sockaddr_type addr = to_sockaddr ();

        if (NULL == inet_ntop ((int)net::domain::INET, &addr.sin_addr, dest, sizeof (dest)))
            net::error::throw_code ();
        return dest;
    }


    template <>
    bool
    address<net::domain::INET>::operator ==(const address<net::domain::INET> &rhs) {
        return m_ip   == rhs.m_ip   &&
               m_mask == rhs.m_mask &&
               m_port == rhs.m_port;
    }
}


///////////////////////////////////////////////////////////////////////////////
std::ostream&
net::operator<< (std::ostream &os, const address<net::domain::INET> &addr) {
    os << addr.to_string () << ":" << addr.port ();
    return os;
}


///////////////////////////////////////////////////////////////////////////////
template <> const address<net::domain::INET>
address<net::domain::INET>::LOOPBACK  ("127.0.0.1", 0);

template <> const address<net::domain::INET>
address<net::domain::INET>::ANY       ("0.0.0.0", 0);

namespace net {
    template class address<net::domain::INET>;
}

//-----------------------------------------------------------------------------
//template <> const address<net::domain::INET6>
//address<net::domain::INET6>::LOOPBACK ("::1", 0);
//
//template <> const address<net::domain::INET6>
//address<net::domain::INET6>::ANY      ("::0", 0);
//
//template class address<net::domain::INET6>;

