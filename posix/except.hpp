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
 * Copyright 2010, 2017,
 *     Danny Robson <danny@nerdcruft.net>
 */

#ifndef CRUFT_UTIL_POSIX_EXCEPT_HPP
#define CRUFT_UTIL_POSIX_EXCEPT_HPP

#include <stdexcept>

namespace util::posix {
    /// An exception class used for reporting errors signalled by errno.
    ///
    /// Ideally this would be named `errno' but that symbol is permitted to
    /// be a macro and significantly complicates symbol resolution either way.
    class error : public std::runtime_error {
    public:
        explicit error (int code);
        error ();

        int code (void) const;
        static int last_code (void);

        static void try_code (void);
        static void try_code (int code);

        static void throw_code [[gnu::noreturn]] (void);
        static void throw_code [[gnu::noreturn]] (int code);

        template <typename T>
        static T try_value (T value)
        {
            if (value < 0)
                throw_code ();
            return value;
        }

    private:
        int m_code;
    };
}

#endif
