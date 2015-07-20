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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_FORMAT_HPP
#define __UTIL_FORMAT_HPP

#include <stdexcept>
#include <string>

namespace util {
    namespace format {
        template <typename ...Args>
        std::string render (const std::string &fmt, Args&&...);

        class error : public std::runtime_error
        { using runtime_error::runtime_error; };

        // value-specifier mismatch
        class value_error : public error
        { using error::error; };

        // malformed format specifier
        class format_error : public error
        { using error::error; };

        // missing format specifier
        class missing_error : public error
        { using error::error; };
    }
}

#include "format.ipp"

#endif
