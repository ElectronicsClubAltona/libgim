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

#ifndef __UTIL_HASH_MURMUR_MURMUR3_HPP
#define __UTIL_HASH_MURMUR_MURMUR3_HPP

#include <array>
#include <cstddef>
#include <cstdint>

// Austin Appleby's MurmurHash3
namespace util::hash::murmur3 {
    uint32_t mix (uint32_t);
    uint64_t mix (uint64_t);

    uint32_t hash_32 (const void *restrict data, size_t len, uint32_t seed);
    std::array<uint32_t,4> hash_128_x86 (const void *restrict data, size_t len, uint32_t seed);
    std::array<uint64_t,2> hash_128_x64 (const void *restrict data, size_t len, uint32_t seed);
}

#endif

