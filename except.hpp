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
 * Copyright 2010 Danny Robson <danny@nerdcruft.net>
 */


#ifndef __EXCEPT_HPP
#define __EXCEPT_HPP

#include "platform.hpp"

#include <stdexcept>

namespace util {
    class input_error : public std::runtime_error {
        public:
            input_error (const std::string &_what):
                    runtime_error (_what)
            { ; }
    };


    class output_error : public std::runtime_error {
        public:
            output_error (const std::string &_what):
                runtime_error (_what)
            { ; }
    };


    class unavailable_error : public std::runtime_error {
        public:
            unavailable_error (const std::string &_what):
                    runtime_error (_what)
            { ; }
    };


    /// An exception class used for reporting errors signalled by errno.
    class errno_error : public std::runtime_error {
    public:
        errno_error (int code);
        errno_error ();

        int code (void) const;
        static int last_code (void);

        static void try_code (void);
        static void try_code (int code);

        static void throw_code [[gnu::noreturn]] (void);
        static void throw_code [[gnu::noreturn]] (int code);

    private:
        int m_code;
    };
}

#if defined(PLATFORM_WIN32)
#include <windows.h>

namespace  util {
    class win32_error : public std::runtime_error {
    public:
        win32_error (DWORD _code);
        win32_error ();

        DWORD code (void) const;
        static DWORD last_code (void);

        static void try_code (void);
        static void try_code (DWORD);

        static void throw_code [[gnu::noreturn]] (void);
        static void throw_code [[gnu::noreturn]] (DWORD);

        static std::string code_string (void);
        static std::string code_string (DWORD);

    private:
        DWORD m_code;
    };
}
#endif

#endif
