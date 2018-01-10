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

#ifndef __UTIL_POSIX_DIR_HPP
#define __UTIL_POSIX_DIR_HPP

#include <dirent.h>

#include <functional>
#include <experimental/filesystem>

namespace util::posix {
    struct dir {
    public:
        explicit dir (const std::experimental::filesystem::path&);
        ~dir ();

        operator DIR* (void);

        template <typename ...Args>
        void
        scan (std::function<void(const std::experimental::filesystem::path&, Args&...)>, Args&...);

        template <typename ...Args>
        void
        scan (void (*) (const std::experimental::filesystem::path&, Args&...), Args&...);

        //entry begin (void) { rewind (); return { readdir (m_handle), m_handle }; }
        //entry end (void) { return { nullptr, m_handle }; }

        void rewind (void);

    private:
        DIR *m_handle;
    };
}

#include "dir.ipp"

#endif
