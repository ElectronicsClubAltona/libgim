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
 * Copyright:
 *      2014-2018, Danny Robson <danny@nerdcruft.net>
 */

#ifndef CRUFT_UTIL_HASH_RIPEMD_HPP
#define CRUFT_UTIL_HASH_RIPEMD_HPP

#include "../view.hpp"

#include <array>


///////////////////////////////////////////////////////////////////////////////
namespace util::hash {
    class RIPEMD {
    public:
        typedef std::array<uint8_t,20> digest_t;

        digest_t operator() (util::view<const uint8_t*>);
    };
}

#endif
