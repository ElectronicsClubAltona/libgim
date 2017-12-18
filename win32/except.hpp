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
 * Copyright 2010, 2017
 *     Danny Robson <danny@nerdcruft.net>
 */

#ifndef CRUFT_UTIL_WIN32_EXCEPT_HPP
#define CRUFT_UTIL_WIN32_EXCEPT_HPP

#include <stdexcept>
#include <windows.h>

namespace util::win32 {
    class error : public std::runtime_error {
    public:
        explicit error (DWORD _code);
        error ();

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
