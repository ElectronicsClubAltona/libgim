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

#ifndef __NET_SOCKET_HPP
#define __NET_SOCKET_HPP

#include <memory>

#include "types.hpp"
#include "address.hpp"


//-----------------------------------------------------------------------------
namespace net {
    template <domain D>
    class socket_domain {
        public:
            typedef address<D> address_type;

        protected:
            socket_t m_fd;

            socket_domain (socket_t _fd);
            socket_domain (const socket_domain<D>&);
            ~socket_domain ();

        public:
            socket_t native (void) const;

            void shutdown (void);

            template <typename T>
            T get_option (level, option);

            void set_option (level, option);
            template <typename T>
            void set_option (level, option, const T &value);

        protected:
            void bind (const address_type&);
    };


    template <domain D, type T>
    class socket;


    template <domain D>
    class socket<D, type::STREAM>: public socket_domain<D> {
        public:
            typedef socket<D, type::STREAM>      socket_type;
            typedef std::unique_ptr<socket_type> socket_ptr;
            typedef address<D>                   address_type;

        protected:
            static const unsigned int DEFAULT_BACKLOG = 5;

            socket (socket_t _fd);

        public:
            socket ();
            socket (const socket_type  &);

            void   send (const uint8_t *restrict, size_t);
            size_t recv (uint8_t *restrict, size_t);

            void connect (const address_type&);

            void listen (const address_type&, unsigned int backlog = DEFAULT_BACKLOG);
            socket_ptr accept (void);

            address_type get_peer (void) const;
    };


    template <domain D>
    class socket<D, type::DGRAM> : public socket_domain<D> {
        public:
            typedef socket<D, type::DGRAM> socket_type;
            typedef address<D>             address_type;

            socket (socket_t _fd);

        public:
            socket ();
            socket (const socket_type  &);

            void send_addr (const address_type&, const uint8_t *restrict, size_t);
            address_type recv_addr (uint8_t *restrict, size_t);
    };
}


#endif // __NET_SOCKET_HPP

