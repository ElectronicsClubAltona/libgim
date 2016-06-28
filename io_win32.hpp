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
 * Copyright 2014-2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_IO_WIN32_HPP
#define __UTIL_IO_WIN32_HPP

#include "./win32/handle.hpp"
#include "./view.hpp"

#include <cstdint>
#include <boost/filesystem/path.hpp>
#include <windows.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

namespace util {
    namespace detail { namespace win32 {
        enum {
            PROT_NONE   = 0,
            PROT_READ   = 1 << 0,
            PROT_EXEC   = 1 << 1,
            PROT_WRITE  = 1 << 2
        };

        class mapped_file {
        public:
            mapped_file (const boost::filesystem::path &path,
                         int fflags = O_RDONLY,
                         int mflags = PROT_READ);

            mapped_file (const mapped_file&) = delete;
            mapped_file& operator= (const mapped_file&) = delete;

            const uint8_t* data (void) const;
            uint8_t*       data (void);

            size_t size  (void) const;
            bool   empty (void) const;

            uint8_t* begin (void);
            uint8_t* end   (void);

            const uint8_t* begin (void) const;
            const uint8_t* end   (void) const;

            const uint8_t* cbegin (void) const;
            const uint8_t* cend   (void) const;

            template <typename T>
            operator util::view<const T *restrict> () const &;

            template <typename T>
            operator util::view<T *restrict> () &;

        private:
            ::util::win32::handle m_file;
            ::util::win32::handle m_mapping;

            std::unique_ptr<uint8_t,BOOL(*)(LPCVOID)> m_data;
            uint64_t m_size;
        };
    } }

    typedef detail::win32::mapped_file mapped_file;
}

#include "io_win32.ipp"

#endif
