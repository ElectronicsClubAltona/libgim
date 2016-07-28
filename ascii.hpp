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

namespace util { namespace ascii {
    ///////////////////////////////////////////////////////////////////////////
    constexpr inline
    bool
    is_digit (char c) noexcept
    {
        return c >= '0' && c <= '9';
    }


    //-------------------------------------------------------------------------
    constexpr inline
    bool
    is_upper (char c) noexcept
    {
        return c >= 'A' && c <= 'Z';
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
} }

#endif
