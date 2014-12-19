/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
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
