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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_STREAM_HPP
#define __UTIL_STREAM_HPP

#include <iostream>
#include "types/bits.hpp"

namespace util {
    namespace stream {
        //---------------------------------------------------------------------
        template <typename T>
        struct numeric
        {
            explicit numeric (T _val): val (_val) { ; }
            T val;
        };

        template <typename T>
        std::ostream& operator<< (std::ostream &os, numeric<T> n)
        {
            static_assert (std::is_fundamental<T>::value,
                           "numeric streamer is intended for chars");

            using integral_t = typename std::conditional<
                std::is_floating_point<T>::value,
                T,
                typename std::conditional<
                    std::is_signed<T>::value,
                    sized_type< intmax_t>::sint,
                    sized_type<uintmax_t>::uint
                >::type
            >::type;

            return os << (integral_t)n.val;
        }

        //---------------------------------------------------------------------
        class null : public std::ostream {
            public:
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


        //---------------------------------------------------------------------
        struct bits {
            bits (uintmax_t value, unsigned count);

            uintmax_t value;
            unsigned  count;
        };

        std::ostream& operator<< (std::ostream&, bits);
    }
}


#endif
