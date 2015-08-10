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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_LIBRARY_POSIX_HPP
#define __UTIL_LIBRARY_POSIX_HPP

#include <string>

namespace util {
    namespace detail {
        class library_posix {
        public:
            library_posix (const char *path);
            library_posix (const std::string &path);
            library_posix (library_posix&&);
            ~library_posix ();

            library_posix (const library_posix&) = delete;
            library_posix& operator= (const library_posix&) = delete;

            void* symbol (const char *name);

        private:
            void *m_handle;

        };
    }

    typedef detail::library_posix library;
}

#endif
