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


#ifndef CRUFT_UTIL_UTF8_HPP
#define CRUFT_UTIL_UTF8_HPP

#include "./view.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>


namespace util::utf8 {
    ///////////////////////////////////////////////////////////////////////////
    using codepoint_t = uint32_t;

    constexpr codepoint_t MAX_CODEPOINT = 0x10FFFF;


    ///////////////////////////////////////////////////////////////////////////
    std::vector<codepoint_t>
    decode (util::view<const std::byte*>);


    //-------------------------------------------------------------------------
    inline auto
    decode (util::view<const char*> data)
    {
        return decode ({
            reinterpret_cast<const std::byte*> (data.cbegin ()),
            reinterpret_cast<const std::byte*> (data.cend   ())
        });
    }

    inline auto
    decode (util::view<const uint8_t*> data)
    {
        return decode ({
            reinterpret_cast<const char*> (data.cbegin ()),
            reinterpret_cast<const char*> (data.cend   ())
        });
    }


    ///////////////////////////////////////////////////////////////////////////
    std::vector<std::byte>
    encode (util::view<const char*>);


    ///////////////////////////////////////////////////////////////////////////
    struct error : public std::exception {};

    struct malformed_error : public error { };
    struct illegal_codepoint : public malformed_error {};

    struct overlong_error  : public error { };
}

#endif
