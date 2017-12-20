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
 * Copyright 2016-2017 Danny Robson <danny@nerdcruft.net>
 */

#include "view.hpp"

#include <algorithm>
#include <iterator>


///////////////////////////////////////////////////////////////////////////////
namespace util {
#define OSTREAM(A)                                                          \
    template <>                                                             \
    std::ostream&                                                           \
    operator<< (std::ostream &os, view<A> a) {                              \
        std::copy (a.cbegin (),                                             \
                   std::find (a.cbegin (), a.cend (), '\0'),                \
                   std::ostream_iterator<decltype(a)::value_type> (os));    \
                                                                            \
        return os;                                                          \
    }

    OSTREAM(const char*)
    OSTREAM(char*)
    OSTREAM(  signed char*)
    OSTREAM(unsigned char*)
    OSTREAM(const   signed char*)
    OSTREAM(const unsigned char*)
    OSTREAM(std::string::const_iterator)
    OSTREAM(std::string::iterator)

#undef OSTREAM
}

