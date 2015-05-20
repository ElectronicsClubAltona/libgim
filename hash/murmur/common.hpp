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

#include <array>
#include <cstddef>
#include <cstdint>

///////////////////////////////////////////////////////////////////////////////
namespace util { namespace hash { namespace murmur {
    uint32_t tail (const uint32_t *restrict data, size_t len);
    uint64_t tail (const uint64_t *restrict data, size_t len);

    std::array<uint32_t,4>
    tail_array (const uint32_t *restrict data, size_t len);

    std::array<uint64_t,2>
    tail_array (const uint64_t *restrict data, size_t len);
} } }
