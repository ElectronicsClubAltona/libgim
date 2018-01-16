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
 * Copyright 2010-2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_IO_POSIX_HPP
#define __UTIL_IO_POSIX_HPP

#include "posix/fd.hpp"

#include "view.hpp"

#include <type_traits>
#include <experimental/filesystem>

#include <sys/mman.h>
#include <fcntl.h>

namespace util {
    namespace detail::posix {
        class mapped_file {
        public:
            using value_type = uint8_t;
            using reference = value_type&;
            using const_reference = const value_type&;
            using iterator = value_type*;
            using const_iterator = const value_type*;
            using difference_type = std::iterator_traits<iterator>::difference_type;
            using size_type = size_t;

            mapped_file (const std::experimental::filesystem::path&,
                         int fflags = O_RDONLY | O_BINARY,
                         int mflags = PROT_READ);
            mapped_file (const util::posix::fd&,
                         int mflags = PROT_READ);

            mapped_file (const mapped_file&) = delete;
            mapped_file& operator= (const mapped_file&) = delete;

            ~mapped_file ();

            bool empty (void) const;

            /// returns the total allocated mapped region in bytes.
            ///
            /// result is typed size_t (rather than a signed type) because we
            /// often use this in conjunction with sizeof and packed structure.
            /// it is greatly simpler to cast to signed where it's actually
            /// required rather than the other way around.
            size_type size (void) const;

            const_iterator data (void) const &;
            iterator       data (void) &;

            iterator begin (void) &;
            iterator end   (void) &;

            const_iterator begin (void) const &;
            const_iterator end   (void) const &;

            const_iterator cbegin (void) const &;
            const_iterator cend   (void) const &;

        private:
            uint8_t *m_data;
            size_t   m_size;
        };
    }

    typedef detail::posix::mapped_file mapped_file;
}

#include "io_posix.ipp"

#endif
