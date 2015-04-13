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
            throw_code [[noreturn]] (int code);

            /// Throw an error corresponding to the most recent error condition. This will check
            /// the current error condition in a platform agnostic manner, and pass on to
            /// throw_code(int). This should be used whenever an error has been detected, rather
            /// than the more normal try_code(errno) due to Windows error reporting quirks.
            static void
            throw_code [[noreturn]] (void);

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
