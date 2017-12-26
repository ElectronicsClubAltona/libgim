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

#ifndef CRUFT_UTIL_POSIX_SOCKET_HPP
#define CRUFT_UTIL_POSIX_SOCKET_HPP

#include "fd.hpp"

#include "except.hpp"

#include <sys/types.h>
#include <sys/socket.h>


namespace util::posix {
     class socket : public fd {
     public:
         explicit socket (int _fd);

         socket (int domain, int type);
         socket (int domain, int type, int protocol);

         socket (util::view<const char*> host, int port);

         socket (const socket&) = delete;
         socket& operator= (const socket&) = delete;

         socket (socket &&rhs);
         socket& operator= (socket &&);

         // the destructor is provided so that we can operate more cleanly on
         // win32 where we must call closesocket instead of the normal close.
         // because windows...
         ~socket ();

         void connect (util::view<const char*> host, int port);
         void shutdown ();

         template <typename ValueT>
         void setoption (int _level, int _name, const ValueT &_value)
         {
            error::try_value (
                setsockopt (
                    native (),
                    _level,
                    _name,
                    &_value, sizeof (_value)
                )
            );
         }
     };
};

#endif
