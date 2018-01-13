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
 * Copyright 2013-2018 Danny Robson <danny@nerdcruft.net>
 */

#ifndef CRUFT_UTIL_HASH_SHA1_HPP
#define CRUFT_UTIL_HASH_SHA1_HPP

#include "../view.hpp"

#include <array>
#include <cstdint>
#include <cstdlib>

///////////////////////////////////////////////////////////////////////////////
namespace util::hash {
    class SHA1 {
    public:
        using digest_t = std::array<uint8_t,20>;
        static const size_t BLOCK_SIZE  = 64;
        static const size_t DIGEST_SIZE = 20;


        //template <typename ...DataT>
        //digest_t
        //operator() (DataT&&...);

        digest_t operator() (util::view<const uint8_t*>, util::view<const uint8_t*>) noexcept;
        digest_t operator() (util::view<const uint8_t*>) noexcept;
    };
}

#endif
