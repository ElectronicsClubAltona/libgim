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
        using runtime_error::runtime_error;
    };


    /// Base class for all type conversion errors
    struct type_error : public error {
        using error::error;
    };


    /// Base class for errors thrown during parsing
    struct parse_error : public error {
        using error::error;

        parse_error (const std::string &_what, size_t _line = 0);

        size_t line;
    };


    /// Base class for errors thrown during schema validation
    struct schema_error : public error {
        using error::error;
    };


    /// Exception class for invalid object indexing
    struct key_error : public error {
        key_error (std::string);
    };
}

#endif
