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

#include <sys/types.h>
#include <dirent.h>

#include <functional>

namespace util { namespace posix {
    //struct entry {
    //public:
    //    entry (dirent *_handle, DIR *_parent): m_handle (_handle), m_parent (_parent) { ; }

    //    const char* name (void) const { return m_handle->d_name; }

    //    entry& operator++ (void) {
    //        dirent *next = readdir (m_parent);
    //        assert (!next || next != m_handle);
    //        m_handle = next;
    //        return *this;
    //    }

    //    entry& operator* (void) { return *this; }
    //    const entry& operator* (void) const;
    //    bool operator!= (entry rhs) const { return m_handle != rhs.m_handle || m_parent != rhs.m_parent; }

    //    struct dirent *m_handle;
    //    DIR *m_parent;
    //};

    struct dir {
    public:
        explicit dir (const char *path);
        ~dir ();

        operator DIR* (void);

        template <typename ...Args>
        void
        scan (std::function<void(const char*, Args&...)>, Args&...);

        template <typename ...Args>
        void
        scan (void (*) (const char*, Args&...), Args&...);

        //entry begin (void) { rewind (); return { readdir (m_handle), m_handle }; }
        //entry end (void) { return { nullptr, m_handle }; }

        void rewind (void);

    private:
        DIR *m_handle;
    };
} }

#include "./dir.ipp"

#endif
