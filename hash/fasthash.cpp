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

#include "fasthash.hpp"


///////////////////////////////////////////////////////////////////////////////
uint64_t
util::hash::fasthash::mix (uint64_t v)
{
    v ^= v >> 23;
    v *= 0x2127599bf4325c37;
    v ^= v >> 47;

    return v;
}


///////////////////////////////////////////////////////////////////////////////
uint64_t
util::hash::fasthash::hash64 (const void *restrict data, size_t len, uint64_t seed)
{
    static const uint64_t m = 0x880355f21e6d1965;

    uint64_t result = seed ^ (len * m);

    auto cursor = reinterpret_cast<const uint64_t*> (data);
    auto last   = cursor + len / sizeof (*cursor);
    for (; cursor < last; ++cursor) {
        result ^= mix (*cursor);
        result *= m;
    }

    size_t remain = len % sizeof (*cursor);
    if (remain) {
        auto tail = reinterpret_cast<const uint8_t*> (cursor);
        
        uint64_t accum = 0;
        for (size_t i = 0; i < remain; ++i)
            accum ^= uint64_t {tail[i]} << i * 8;

        result ^= mix (accum);
        result *= m;
    }

    return mix (result);
} 


//-----------------------------------------------------------------------------
uint32_t
util::hash::fasthash::hash32 (const void *restrict data, size_t len, uint32_t seed)
{
    uint64_t h = hash64 (data, len, seed);
    return h - (h >> 32);
}

