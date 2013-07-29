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
 * Copyright 2013 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_ZLIB_HPP
#define __UTIL_ZLIB_HPP

#include <stdexcept>
#include <string>

#include <zlib.h>


namespace util {
    namespace zlib {
        extern const char* version (void);
        extern const char* code_to_string (int);

        extern void compress (uint8_t       *dst, size_t dst_len,
                              const uint8_t *src, size_t src_len,
                              int level);

        extern size_t compress_bound (size_t len);

        extern size_t uncompress (uint8_t       *dst, size_t dst_len,
                                  const uint8_t *src, size_t src_len);


        class stream {
            public:

            public:
                stream();
                ~stream();

                int deflate (bool flush = false);
                int inflate (bool flush = false);
        };


        class error : public std::runtime_error {
            public:
                static void try_code   (int code);
                static void throw_code (int code);

                error(const std::string&);
        };


        template <int CODE>
        class code_error : public error {
            public:
                code_error(void):
                    error (code_to_string (CODE))
                { ; }
        };
    }
}

#endif
