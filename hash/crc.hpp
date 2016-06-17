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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_HASH_CRC_HPP
#define __UTIL_HASH_CRC_HPP

#include <cstdint>
#include <cstdlib>

namespace util { namespace hash {
    uint32_t crc32 (const void *restrict data, size_t bytes) noexcept;
    uint32_t crc32 (const uint8_t *restrict first, const uint8_t *restrict last) noexcept;
} }
#endif

