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

#ifndef __CRUFT_UTIL_ASCII_HPP
#define __CRUFT_UTIL_ASCII_HPP

#include "./annotation.hpp"

#include <cstdint>
#include <stdexcept>

namespace util::ascii {
    ///////////////////////////////////////////////////////////////////////////
    constexpr inline
    bool
    is_digit (char c) noexcept
    {
        return c >= '0' && c <= '9';
    }

    //-------------------------------------------------------------------------
    constexpr inline
    uint8_t
    to_integer (char c)
    {
        return likely (is_digit (c)) ?
              c - '0'
            : throw std::invalid_argument ("character is not a digit");
    }


    //-------------------------------------------------------------------------
    constexpr inline
    bool
    is_upper (char c) noexcept
    {
        return c >= 'A' && c <= 'Z';
    }


    //-------------------------------------------------------------------------
    constexpr inline
    bool
    is_lower (char c) noexcept
    {
        return c >= 'a' && c <= 'z';
    }


    //-------------------------------------------------------------------------
    constexpr inline
    bool
    is_hex (const char c) noexcept
    {
        switch (c) {
        case '0'...'9': return true;
        case 'a'...'f': return true;
        case 'A'...'F': return true;
        }

        return false;
    }


    //-------------------------------------------------------------------------
    constexpr inline
    unsigned
    from_hex (char c)
    {
        return c >= '0' && c <= '9' ? (c - '0'     ) :
               c >= 'a' && c <= 'f' ? (c - 'a' + 10) :
               c >= 'A' && c <= 'F' ? (c - 'A' + 10) :
               throw std::invalid_argument ("character is not hexademical");
    }


    ///////////////////////////////////////////////////////////////////////////
    constexpr inline
    char
    to_upper (char c) noexcept
    {
        return c - 'a' + 'A';
    }


    //-------------------------------------------------------------------------
    constexpr inline
    char
    to_lower (char c) noexcept
    {
        return c - 'A' + 'a';
    }


    ///////////////////////////////////////////////////////////////////////////
    constexpr inline
    bool
    is_space (char c)
    {
        switch (c) {
        case ' ':
        case '\f':
        case '\n':
        case '\r':
        case '\t':
        case '\v':
            return true;

        default:
            return false;
        }
    }
}

#endif
