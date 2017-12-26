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
 * Copyright 2017 Danny Robson <danny@nerdcruft.net>
 */

#include "socket.hpp"

#include "except.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

using util::posix::socket;


///////////////////////////////////////////////////////////////////////////////
class lookup {
public:
    lookup (const util::view<const char*> host, int port):
        m_addresses (nullptr, freeaddrinfo)
    {
        struct ::addrinfo hints {};
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        const struct {
            std::string host;
            std::string port;
        } strings {
            .host = std::string (std::begin (host), std::end (host)),
            .port = std::to_string (port)
        };

        addrinfo* _addresses;
        util::posix::eai::try_code (
            getaddrinfo (
                strings.host.c_str (),
                strings.port.c_str (),
                &hints,
                &_addresses
            )
        );

        m_addresses.reset (_addresses);
    };

    auto begin (void) const { return m_addresses.get (); }
    auto end   (void) const { return nullptr; }

private:
    std::unique_ptr<addrinfo,void(*)(addrinfo*)> m_addresses;
};


///////////////////////////////////////////////////////////////////////////////
static util::posix::fd
connect_host (const util::view<const char*> host, int port)
{
    for (const auto &info: lookup { host, port }) {
        util::posix::fd sock {
            socket (info.ai_family, info.ai_socktype, info.ai_protocol)
        };
        if (sock == -1)
            continue;

        if (connect (sock, info.ai_addr, info.ai_addrlen))
            continue;

        return sock;
    }

    throw std::runtime_error ("unable to connect");
}


///////////////////////////////////////////////////////////////////////////////
socket::socket (int _fd):
    fd (_fd)
{ ; }


//-----------------------------------------------------------------------------
socket::socket (int domain, int type):
    socket (domain, type, 0)
{ ; }


//-----------------------------------------------------------------------------
socket::socket (int domain, int type, int protocol):
    fd (::socket (domain, type, protocol))
{ ; }


//-----------------------------------------------------------------------------
socket::socket (util::view<const char*> host, int port):
    fd (connect_host (host, port))
{ ; }


//-----------------------------------------------------------------------------
socket::~socket ()
{ ; }


//-----------------------------------------------------------------------------
socket::socket (socket &&rhs):
    fd (std::move (rhs))
{ ; }


//-----------------------------------------------------------------------------
class socket&
socket::operator= (socket &&rhs)
{
    fd::operator= (std::move (rhs));
    return *this;
}


///////////////////////////////////////////////////////////////////////////////
void
socket::connect (util::view<const char*> host, int port)
{
    const lookup l { host, port };
    for (auto cursor = l.begin (); cursor != l.end (); cursor = cursor->ai_next) {
        if (!::connect (*this, cursor->ai_addr, cursor->ai_addrlen))
            return;
    }

    throw std::runtime_error ("unable to reconnect");
}
