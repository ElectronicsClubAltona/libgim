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

#ifndef __NET_TYPES_HPP
#define __NET_TYPES_HPP


#if defined(HAVE_WINSOCK2_H)
    #include <winsock2.h>
#else
    #include <netinet/in.h>
    #include <netinet/tcp.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <cstring>
#endif

#include <string>
#include <stdexcept>


namespace net {
    /// Cross platform socket type to deal with Winsock2
#if defined(HAVE_WINSOCK2_H)
    typedef SOCKET socket_t;
#else
    typedef int socket_t;
    const socket_t INVALID_SOCKET = -1;
#endif

    /// Defines the protocol family, or communication domain of a socket (see `man socket').
    enum class domain : int {
#define DEFINE_DOMAIN(V) \
        V = AF_##V

        DEFINE_DOMAIN(UNIX),
        //DEFINE_DOMAIN(LOCAL),
        DEFINE_DOMAIN(INET),
        DEFINE_DOMAIN(INET6),
        DEFINE_DOMAIN(IPX),
        //DEFINE_DOMAIN(NETLINK),
        //DEFINE_DOMAIN(X25),
        //DEFINE_DOMAIN(AX25),
        //DEFINE_DOMAIN(ATMPVC),
        DEFINE_DOMAIN(APPLETALK)
        //DEFINE_DOMAIN(PACKET)

#undef DEFINE_DOMAIN
    };


    /// Specifies the communication semantics of a socket; how a socket deals with data.
    enum class type : int {
#define DEFINE_TYPE(V) \
        V = SOCK_##V

        DEFINE_TYPE(STREAM),
        DEFINE_TYPE(DGRAM),
        DEFINE_TYPE(SEQPACKET),
        DEFINE_TYPE(RAW),
        DEFINE_TYPE(RDM)
        //DEFINE_TYPE(PACKET)

#undef DEFINE_TYPE
    };


    /// Indicates the wire transmission protocol to use.
    /// 
    /// This DOES NOT mean what you think it does! It is included for completeness sake, and unless
    /// you're doing something funky with the C API you really just want DEFAULT.
    /// Values retrieved from /etc/protocols
    enum class protocol : int {
        DEFAULT = 0,

        IP   =  0,
        ICMP =  1,
        TCP  =  6,
        UDP  = 17,
        IPV6 = 41
    };


    protocol
    string_to_protocol (const std::string&);

    protocol
    string_to_protocol (const char *name);

    std::string
    protocol_to_string (protocol);


    enum class option : int {
#define DEFINE_OPTION(V) \
        V = SO_##V

        DEFINE_OPTION(DEBUG),
        DEFINE_OPTION(REUSEADDR),
        DEFINE_OPTION(TYPE),
        //DEFINE_OPTION(ERROR),
        DEFINE_OPTION(DONTROUTE),
        DEFINE_OPTION(BROADCAST),
        //DEFINE_OPTION(SNDBUF),
        DEFINE_OPTION(RCVBUF),
        //DEFINE_OPTION(SNDBUFFORCE),
        //DEFINE_OPTION(RCVBUFFORCE),
        DEFINE_OPTION(KEEPALIVE),
        DEFINE_OPTION(OOBINLINE),
        //DEFINE_OPTION(NO_CHECK),
        //DEFINE_OPTION(PRIORITY),
        DEFINE_OPTION(LINGER),
        //DEFINE_OPTION(BSDCOMPAT),

        //DEFINE_OPTION(PASSCRED),
        //DEFINE_OPTION(PEERCRED),
        DEFINE_OPTION(RCVLOWAT),
        DEFINE_OPTION(SNDLOWAT),
        DEFINE_OPTION(RCVTIMEO),
        DEFINE_OPTION(SNDTIMEO),

        //DEFINE_OPTION(BINDTODEVICE),

        //DEFINE_OPTION(ATTACH_FILTER),
        //DEFINE_OPTION(DETACH_FILTER),

        //DEFINE_OPTION(PEERNAME),
        //DEFINE_OPTION(TIMESTAMP),

        DEFINE_OPTION(ACCEPTCONN),

        //DEFINE_OPTION(PEERSEC),
        //DEFINE_OPTION(PASSSEC),
        //DEFINE_OPTION(TIMESTAMPNS),

        //DEFINE_OPTION(MARK),

        //DEFINE_OPTION(TIMESTAMPING)
#undef DEFINE_OPTION

#define DEFINE_OPTION(V) \
        V = TCP_##V

        //DEFINE_OPTION(CORK),
        //DEFINE_OPTION(DEFER_ACCEPT),
        //DEFINE_OPTION(INFO),
        //DEFINE_OPTION(KEEPCNT),
        //DEFINE_OPTION(KEEPIDLE),
        //DEFINE_OPTION(KEEPINTVL),
        //DEFINE_OPTION(LINGER2),
        //DEFINE_OPTION(MAXSEG),
        DEFINE_OPTION(NODELAY)
        //DEFINE_OPTION(QUICKACK),
        //DEFINE_OPTION(SYNCNT),
        //DEFINE_OPTION(WINDOW_CLAMP)
#undef DEFINE_OPTION
    };


    enum class level : int {
        SOCKET = SOL_SOCKET,
        TCP    = IPPROTO_TCP
    };
}

#endif // __NET_TYPES_HPP

