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

#ifndef __UTIL_JSON_EXCEPT_HPP
#define __UTIL_JSON_EXCEPT_HPP

#include <stdexcept>
#include <string>
#include <cstdlib>

namespace json {
    /// The base class for all exceptions throw directly by the json namespace.
    struct error : public std::runtime_error {
        explicit error (const std::string &what):
            runtime_error (what)
        { ; }
    };


    /// Base class for all type conversion errors
    struct type_error : public error {
        explicit type_error (const std::string &what):
            error (what)
        { ; }
    };


    /// Base class for errors thrown during parsing
    struct parse_error : public error {
        explicit parse_error (const std::string &_what, size_t _line = 0);

        const char* what (void) const noexcept override;

        size_t line;
        std::string desc;
    };


    /// Base class for errors thrown during schema validation
    struct schema_error : public error {
        explicit schema_error (const std::string &what):
            error (what)
        { ; }
    };


    /// Exception class for invalid object indexing
    struct key_error : public error {
        explicit key_error (std::string);

        std::string key;
    };
}

#endif
