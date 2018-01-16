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

using util::hash::fasthash;


///////////////////////////////////////////////////////////////////////////////
template <>
uint64_t
fasthash<uint64_t>::operator() (uint64_t seed, const util::view<const uint8_t*> data) const
{
    static const uint64_t m = 0x880355f21e6d1965;

    uint64_t result = seed ^ (data.size () * m);

    auto cursor = reinterpret_cast<const uint64_t*> (data.begin ());
    auto last   = cursor + data.size () / sizeof (*cursor);
    for (; cursor < last; ++cursor) {
        result ^= mix (*cursor);
        result *= m;
    }

    size_t remain = data.size () % sizeof (*cursor);
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
template <>
uint32_t
fasthash<uint32_t>::operator() (uint64_t seed, const util::view<const uint8_t*> data) const
{
    auto h = fasthash<uint64_t> {} (seed, data);
    return (h & 0xffffffff) - (h >> 32);
}

