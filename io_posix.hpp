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
 * Copyright 2010-2014 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_IO_POSIX_HPP
#define __UTIL_IO_POSIX_HPP

#include "io.hpp"

namespace util {
    namespace detail { namespace posix {
        class mapped_file {
            private:
                fd_ref   m_fd;
                uint8_t *m_data;
                size_t   m_size;

                void load_fd (access_t);

            protected:
                int access_to_flags (access_t);

            public:
                mapped_file (const boost::filesystem::path &path, access_t access = ACCESS_READ);

                mapped_file (const mapped_file&) = delete;
                mapped_file& operator= (const mapped_file&) = delete;

                ~mapped_file ();

                const uint8_t* data (void) const;
                uint8_t*       data (void);
                size_t         size (void) const;

                uint8_t*       begin (void);
                uint8_t*       end   (void);

                const uint8_t* cbegin (void) const;
                const uint8_t* cend   (void) const;
        };
        
    } }

    typedef detail::posix::mapped_file mapped_file;
}

#endif
