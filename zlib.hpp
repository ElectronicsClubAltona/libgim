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
