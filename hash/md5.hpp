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
 * Copyright 2013 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_HASH_MD5_HPP
#define __UTIL_HASH_MD5_HPP

#include "../view.hpp"

#include <array>
#include <cstdint>
#include <cstdlib>


///////////////////////////////////////////////////////////////////////////////
namespace util::hash {
    class MD5 {
    public:
        using digest_t = std::array<uint8_t,16>;
        static const size_t BLOCK_SIZE  = 64;
        static const size_t DIGEST_SIZE = 16;

    public:
        digest_t operator() (util::view<const uint8_t*>) const noexcept;
        digest_t operator() (util::view<const uint8_t*>,util::view<const uint8_t*>) const noexcept;
        digest_t operator() (util::view<const uint8_t*>,util::view<const uint8_t*>,util::view<const uint8_t*>) const noexcept;

    private:
        std::array<uint32_t,4>
        transform (
            const std::array<uint32_t,4> &ABCD,
            const std::array<uint32_t,16> &X
        ) const noexcept;
    };
}

#endif
