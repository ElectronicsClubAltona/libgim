
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

#include "murmur2.hpp"

#include "../../debug.hpp"
#include "common.hpp"


///////////////////////////////////////////////////////////////////////////////
uint32_t
util::hash::murmur2::hash_32 (const void *restrict key,
                              size_t len,
                              uint32_t seed)
{
    CHECK (key);

    // setup
    constexpr auto m = detail::constants<uint32_t>::m;
    uint32_t h = seed ^ (len & 0xffffffff);

    // body
    auto cursor = reinterpret_cast<const uint32_t*> (key);
    auto last   = cursor + len / sizeof (uint32_t);
    for (; cursor < last; ++cursor)
        h = mix (h, *cursor);

    // tail
    if (len % sizeof (uint32_t)) {
        h ^= murmur::tail<uint32_t> (reinterpret_cast<const uint8_t*> (cursor), len);
        h *= m;
    }

    // finalise
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}


//-----------------------------------------------------------------------------
uint64_t
util::hash::murmur2::hash_64 (const void *restrict key,
                              size_t len,
                              uint64_t seed)
{
    // setup
    constexpr auto m = detail::constants<uint64_t>::m;
    constexpr auto r = detail::constants<uint64_t>::r;

    uint64_t h = seed ^ (len * m);

    // body
    auto cursor = reinterpret_cast<const uint64_t*> (key);
    auto last   = cursor + len / sizeof (uint64_t);
    for (; cursor < last; ++cursor)
        h = mix (h, *cursor);

    // tail
    if (len % sizeof (uint64_t)) {
        h ^= murmur::tail<uint64_t> (reinterpret_cast<const uint8_t*> (cursor), len);
        h *= m;
    }

    // finalise
    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}


