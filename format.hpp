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
 * Copyright 2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_FORMAT_HPP
#define __UTIL_FORMAT_HPP

#include <stdexcept>
#include <string>

namespace util { namespace format {
    //-------------------------------------------------------------------------
    // render a format string using the provided values.
    //
    // we deliberately only take char[] formats so as to promote the use of
    // only literal strings as format strings.
    template <typename ...Args, size_t N>
    std::string
    render (const char (&fmt)[N], const Args&...);

    //-------------------------------------------------------------------------
    class error : public std::runtime_error
    { using runtime_error::runtime_error; };

    // value-specifier mismatch
    class value_error : public error
    { using error::error; };

    struct conversion_error : public error
    { using error::error; };

    struct length_error : public error
    { using error::error; };

    // malformed format specifier
    class syntax_error : public error
    { using error::error; };

    template <typename ValueT>
    class invalid_specifier : error {
    public:
        using value_type = ValueT;

        invalid_specifier (char specifier);

        char specifier (void) const;

    private:
        char m_specifier;
    };

    // missing format specifier
    class missing_error : public error
    {
    public:
        missing_error ():
            error ("missing argument for specifier")
        { ; }
    };
} }

#include "format.ipp"

#endif
