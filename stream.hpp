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
 * Copyright 2011-2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_STREAM_HPP
#define __UTIL_STREAM_HPP

#include <iostream>

namespace util {
    namespace stream {
        ///////////////////////////////////////////////////////////////////////
        class null_streambuf : public std::basic_streambuf<char> {
        public:
            virtual ~null_streambuf () { ; }
        };


        ///////////////////////////////////////////////////////////////////////
        class null_ostream : public std::basic_ostream<char> {
        public:
            virtual ~null_ostream () { ; }

            std::ostream & put   (char c);
            std::ostream & write (const char *s, std::streamsize n);

            std::streampos tellp (void);
            std::ostream & seekp (std::streampos pos);
            std::ostream & seekp (std::streamoff off,
                                  std::ios_base::seekdir dir);

            std::ostream & flush (void);

            bool good (void) const;
            bool bad  (void) const;
            bool eof  (void) const;
            bool fail (void) const;
        };


        ///////////////////////////////////////////////////////////////////////
        struct bits {
            bits (uintmax_t value, unsigned count);

            uintmax_t value;
            unsigned  count;
        };

        std::ostream& operator<< (std::ostream&, bits);
    }
}


#endif
