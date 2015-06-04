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

#include "socket.hpp"

#include "../debug.hpp"
#include "../except.hpp"
#include "../types/casts.hpp"
#include "../log.hpp"
#include "except.hpp"

#if defined(HAVE_WINSOCK2_H)
    #include <winsock2.h>
#else
    #include <sys/socket.h>
#endif

#include <sys/types.h>
#include <unistd.h>

using namespace net;

//-----------------------------------------------------------------------------
template <domain D>
socket_domain<D>::socket_domain (socket_t _fd):
    m_fd (_fd)
{
#ifdef __WIN32
#else
    CHECK_GE (m_fd, 0);
#endif
}


#if defined(HAVE_WINSOCK2_H)
    // TODO: Make this not retarded. Fucking Windows.
    #define dup(X) (X)

    // Winsock has incorrect return and parameter types (according to POSIX)
    // so we need some wrappers around the system implementations that casts
    // to expected types. Mainly int vs ssize_t returns, and int vs size_t
    // parameters.
    static_assert(sizeof(int) <= sizeof(ssize_t), "int != ssize_t");
    static_assert(sizeof(int) <= sizeof( size_t), "int !=  size_t");

    ssize_t recv(socket_t _socket, void *_buf, size_t _len, int _flags)
    	{ return (ssize_t)::recv(_socket, (char*)_buf, (int)_len, _flags); }

    ssize_t recvfrom(socket_t _socket, void *_buf, size_t _len, int _flags, struct sockaddr *_sockaddr, socklen_t *_socklen)
    	{ return (ssize_t)::recvfrom(_socket, (char*)_buf, (int)_len, _flags, _sockaddr, (int*)_socklen); }

    ssize_t sendto(socket_t _socket, const void *_buf, size_t _len, int _flags, const struct sockaddr *_sockaddr, socklen_t _socklen)
	{ return (ssize_t)::sendto(_socket, (const char*)_buf, (int)_len, _flags, _sockaddr, (int)_socklen); }

    ssize_t send(socket_t _socket, const void *_buf, size_t _len, int _flags)
	{ return (ssize_t)::send(_socket, (const char*)_buf, (int)_len, _flags); }
#else
    #define closesocket(X) close(X)
#endif


template <domain D>
socket_domain<D>::socket_domain (const socket_domain<D> &rhs):
    m_fd (dup (rhs.m_fd))
{ ; }


template <domain D>
socket_domain<D>::~socket_domain () {
    if (closesocket (m_fd) < 0) {
        LOG_DEBUG ("closesocket: %s", strerror (errno));
    }
}


template <domain D>
socket_t
socket_domain<D>::native (void) const
    { return m_fd; }


template <domain D>
void
socket_domain<D>::shutdown (void) {
#if defined(HAVE_WINSOCK2_H)
    #define SHUT_SEND SD_SEND
    #define SHUT_RECV SD_RECEIVE
    #define SHUT_RDWR SD_BOTH
#endif

    if (::shutdown (m_fd, SHUT_RDWR) < 0)
        net::error::throw_code ();
}


template <domain D>
template <typename T>
T
socket_domain<D>::get_option (level, option) {
    not_implemented ();
    return T ();
}


template <domain D>
void
socket_domain<D>::set_option (level _level, option _option) {
    if (setsockopt (this->m_fd, (int)_level, (int)_option, NULL, 0))
        net::error::throw_code ();
}


template <domain D>
template <typename T>
void
socket_domain<D>::set_option (level _level, option _option, const T &value) {
    if (setsockopt (this->m_fd, (int)_level, (int)_option, (const char*)&value, sizeof (value)))
        net::error::throw_code ();
}


template <domain D>
void
socket_domain<D>::bind (const address_type &addr) {
    typename address_type::sockaddr_type addr_in = addr.to_sockaddr ();

    if (::bind (m_fd, (sockaddr *)&addr_in, sizeof (addr_in)) != 0)
        net::error::throw_code ();
}

//-----------------------------------------------------------------------------
template <domain D>
net::socket<D, type::STREAM>::socket (socket_t _fd):
    net::socket_domain<D> (_fd)
{ ; }


template <domain D>
net::socket<D, type::STREAM>::socket ():
    net::socket_domain<D> (::socket ((int)D, (int)type::STREAM, (int)protocol::DEFAULT))
{ ; }


template <domain D>
net::socket<D, type::STREAM>::socket (const socket_type &rhs):
    socket_domain<D> (rhs)
{ ; }


template <domain D>
void
net::socket<D, type::STREAM>::send (const uint8_t *restrict data, size_t len) {
    CHECK (data != NULL);
    CHECK_GT (len, 0);

    for (size_t sent = 0; sent < len; ) {
        ssize_t result = ::send (this->m_fd, static_cast<const void *>(data + sent), len - sent, 0);

        if (result < 0)
            net::error::throw_code ();

        sent += sign_cast<size_t> (result);
    }
}


template <domain D>
size_t
net::socket<D, type::STREAM>::recv (uint8_t *restrict data, size_t len) {
    CHECK (data != NULL);
    CHECK_GT (len, 0);

    ssize_t received = ::recv (this->m_fd, data, len, 0);
    if (received < 0)
        net::error::throw_code ();

    return sign_cast<size_t> (received);
}


template <domain D>
void
net::socket<D, type::STREAM>::connect (const address_type &_addr) {
    typename address_type::sockaddr_type addr (_addr.to_sockaddr ());

    if (::connect (this->m_fd, reinterpret_cast<sockaddr *> (&addr), sizeof (addr)) < 0)
        net::error::throw_code ();
}


template <domain D>
void
net::socket<D, type::STREAM>::listen (const address_type &_addr, unsigned int _backlog) {
    this->bind (_addr);

    if (::listen (this->m_fd, sign_cast<int>(_backlog)) != 0)
        net::error::throw_code ();
}


template <domain D>
typename net::socket<D, type::STREAM>::socket_ptr
net::socket<D, type::STREAM>::accept (void) {
    socket_t newfd = ::accept (this->m_fd, NULL, 0);
    if (newfd == INVALID_SOCKET)
        net::error::throw_code ();

    return socket_ptr(new socket<D, type::STREAM> (newfd));
}


template <domain D>
typename net::socket<D, type::STREAM>::address_type
net::socket<D, type::STREAM>::get_peer (void) const {
    typename address_type::sockaddr_type addr;
    socklen_t addr_len;

    if (getpeername (this->m_fd, (sockaddr*)&addr, &addr_len))
        net::error::throw_code ();

    CHECK (addr_len == sizeof (addr));
    return typename net::socket<D,type::STREAM>::address_type (addr);
}


//-----------------------------------------------------------------------------
template <domain D>
net::socket<D, type::DGRAM>::socket (socket_t _fd):
    net::socket_domain<D> (_fd)
{ ; }


template <domain D>
net::socket<D, type::DGRAM>::socket ():
    net::socket_domain<D> (::socket ((int)D, (int)type::DGRAM, (int)protocol::DEFAULT))
{ ; }


template <domain D>
net::socket<D, type::DGRAM>::socket (const socket_type &rhs):
    net::socket_domain<D> (rhs)
{ ; }


template <domain D>
void
net::socket<D, type::DGRAM>::send_addr (const address_type      &addr,
                                        const uint8_t *restrict  data,
                                        size_t                   len) {
    CHECK (data != NULL);
    CHECK_GT (len, 0);

    typename address_type::sockaddr_type addr_in = addr.to_sockaddr ();

    ssize_t sent = ::sendto (this->m_fd, data, len, 0, (sockaddr *)&addr_in, sizeof (addr_in));
    if (sent < 0)
        net::error::throw_code ();

    CHECK_EQ (sign_cast<size_t>(sent), len);
}


template <domain D>
typename net::socket<D, type::DGRAM>::address_type
net::socket<D, type::DGRAM>::recv_addr (uint8_t *restrict data,
                                        size_t            len) {
    CHECK (data != NULL);
    CHECK_GT (len, 0);

    typename address_type::sockaddr_type addr_in;
    socklen_t                            addr_len = sizeof (addr_in);

    ssize_t recvd = recvfrom (this->m_fd, data, len, 0, (sockaddr *)&addr_in, &addr_len);
    CHECK_EQ (sizeof (addr_in), addr_len);
    if (recvd < 0)
        net::error::throw_code ();

    return net::socket<D,type::DGRAM>::address_type (addr_in);
}

//-----------------------------------------------------------------------------
template class net::socket_domain<domain::INET>;
template void net::socket_domain<domain::INET>::set_option<int>(level, option, const int&);

template class net::socket<domain::INET, type::STREAM>;
template class net::socket<domain::INET, type::DGRAM>;

