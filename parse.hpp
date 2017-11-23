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
 * Copyright 2017 Danny Robson <danny@nerdcruft.net>
 */

#ifndef CRUFT_UTIL_PARSE_HPP
#define CRUFT_UTIL_PARSE_HPP

#include <cstring>
#include <string>
#include <iterator>


namespace util {
    ///////////////////////////////////////////////////////////////////////////
    /// extracts an instance of a native type T from the string [first, last).
    ///
    /// throws std::invalid_argument when the type cannot be parsed.
    template <typename T>
    T
    parse (const char *first, const char *last);


    //-------------------------------------------------------------------------
    template <typename T>
    T
    parse (const char *str)
    {
        return parse<T> (str, str + strlen (str));
    }


    //-------------------------------------------------------------------------
    template <typename T>
    T
    parse (const std::string &str)
    {
        return parse<T> (std::cbegin (str), std::cend (str));
    }
}

#endif
