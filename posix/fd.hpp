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
 * Copyright 2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __CRUFT_UTIL_POSIX_FD_HPP
#define __CRUFT_UTIL_POSIX_FD_HPP

#include <sys/types.h>
#include <sys/stat.h>

namespace util::posix {
    ///------------------------------------------------------------------------
    /// A simple RAII wrapper for file descriptors
    class fd {
    public:
        fd (const char *path, int flags);
        fd (const char *path, int flags, mode_t);

        fd (fd &&);
        explicit fd (const fd&);
        explicit fd (int);

        ~fd ();

        struct ::stat stat (void) const;

        operator int (void) const;

        static const fd INVALID;

    private:
        int m_fd;
    };
}


#endif
