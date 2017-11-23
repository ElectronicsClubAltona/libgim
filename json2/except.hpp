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

#ifndef CRUFT_UTIL_JSON2_EXCEPT_HPP
#define CRUFT_UTIL_JSON2_EXCEPT_HPP

#include <exception>


namespace util::json2 {
    struct error : public std::exception {};


    struct parse_error : public error {
        parse_error (const char *_position):
            position (_position)
        { ; }

        const char *position;
    };


    struct overrun_error : public parse_error {
        using parse_error::parse_error;
    };
}
#endif
