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

#ifndef __NET_EXCEPT_HPP
#define __NET_EXCEPT_HPP

#if defined(HAVE_WINSOCK2_H)
    #include <winsock2.h>
#else
    #include <cerrno>
    #include <cstring>
#endif

#include <string>
#include <stdexcept>

#include "../annotations.hpp"


//-----------------------------------------------------------------------------
namespace net {
    class error : public std::runtime_error {
        protected:
            error (const std::string &);
            error (int code);

            static std::string
            code_to_string (int code);

        public:
            /// Throw an error corresponding the a given code. Code must be a valid error code,
            /// not success otherwise the application will (at best) abort.
            static void
            throw_code (int code) terminal;

            /// Throw an error corresponding to the most recent error condition. This will check
            /// the current error condition in a platform agnostic manner, and pass on to
            /// throw_code(int). This should be used whenever an error has been detected, rather
            /// than the more normal try_code(errno) due to Windows error reporting quirks.
            static void
            throw_code (void) terminal;

            static void
            try_code (int code);

            static void
            try_code (void);

            static int
            last_code (void);
    };

    
    template <int CODE>
    class error_code : public error {
        public:
            error_code ();

            int code (void) const;
    };
}

#endif // __NET_EXCEPT_HPP
