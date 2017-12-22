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

#ifndef __CRUFT_UTIL_POSIX_FD_HPP
#define __CRUFT_UTIL_POSIX_FD_HPP

#include "../view.hpp"

#include <sys/types.h>
#include <sys/stat.h>

#include <experimental/filesystem>

namespace util::posix {
    ///------------------------------------------------------------------------
    /// A simple RAII wrapper for file descriptors
    class fd {
    public:
        ///////////////////////////////////////////////////////////////////////
        fd (const std::experimental::filesystem::path &path, int flags);
        fd (const std::experimental::filesystem::path &path, int flags, mode_t);

        fd (fd &&);

        // The int constructor steals the fd. So don't pass in something that
        // you don't want closed at destruct time. This should really only be
        // used when interfacing with results of syscalls that we don't wrap.
        explicit fd (int);

        // copy constructors are removed in favour of explicit calls to dup.
        // This should reduce unexpected or expensive copies as much as
        // possible; one should not be doing this unless it is absolutely
        // required.
        fd (const fd&) = delete;
        fd dup (void) const;
        static fd dup (int);

        ~fd ();

        ///////////////////////////////////////////////////////////////////////
        struct ::stat stat (void) const;

        //---------------------------------------------------------------------
        [[gnu::warn_unused_result]] ssize_t read (void *buf, size_t count);
        [[gnu::warn_unused_result]] ssize_t read (util::view<char*>);
        [[gnu::warn_unused_result]] ssize_t read (util::view<std::byte*>);

        [[gnu::warn_unused_result]] ssize_t write (const void *buf, size_t count);
        [[gnu::warn_unused_result]] ssize_t write (util::view<const char*>);
        [[gnu::warn_unused_result]] ssize_t write (util::view<const std::byte*>);

        //---------------------------------------------------------------------
        [[gnu::warn_unused_result]] off_t lseek (off_t offset, int whence);

        ///////////////////////////////////////////////////////////////////////
        operator int (void) const;
        int native (void) { return m_fd; }

    private:
        int m_fd;
    };
}


#endif
