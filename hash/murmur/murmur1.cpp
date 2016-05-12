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

#include "murmur1.hpp"

#include "common.hpp"
#include "../../debug.hpp"

//-----------------------------------------------------------------------------
uint32_t
util::hash::murmur1::mix (uint32_t h, uint32_t k)
{
    static const uint32_t m = 0xc6a4a793;

    h += k;
    h *= m;
    h ^= h >> 16;

    return h;
}


//-----------------------------------------------------------------------------
uint32_t
util::hash::murmur1::hash_32 (const void *restrict data,
                              size_t len,
                              uint32_t seed)
{
    CHECK (data);

    static const uint32_t m = 0xc6a4a793;
    uint32_t h = seed ^ (uint32_t (len) * m);

    // mix the body
    auto cursor = reinterpret_cast<const uint32_t*> (data);
    auto last   = cursor + len / sizeof (uint32_t);
    for (; cursor < last; ++cursor)
        h = mix (h, *cursor);

    // mix the tail
    if (len % sizeof (uint32_t))
        h = mix (h, murmur::tail (cursor, len));

    // finalise
    h *= m; h ^= h >> 10;
    h *= m; h ^= h >> 17;

    return h;
}

