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
 * Copyright 2010-2015, Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_HASH_HPP
#define __UTIL_HASH_HPP

#include "hash/murmur/murmur2.hpp"

#include <cstdint>
#include <cstdlib>

namespace util::hash {
    constexpr std::uint32_t mix (std::uint32_t a, std::uint32_t b) { return murmur2::mix (a, b); }
    constexpr std::uint64_t mix (std::uint64_t a, std::uint64_t b) { return murmur2::mix (a, b); }
}

#endif
