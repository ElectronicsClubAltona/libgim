
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
template <typename T> struct constants { };

template <>
struct constants<uint32_t> {
    static const uint32_t m = 0x5bd1e995;
    static const  int32_t r = 24;
};

template <>
struct constants<uint64_t> {
    static const uint64_t m = 0xc6a4a7935bd1e995;
    static const  int64_t r = 47;
};


/////////////////////////////////////////////////////////////////////
uint32_t
util::hash::murmur2::mix (uint32_t h, uint32_t k)
{
    static const uint32_t m = constants<uint32_t>::m;
    static const uint32_t r = constants<uint32_t>::r;

    k *= m;
    k ^= k >> r;
    k *= m;

    h *= m;
    h ^= k;

    return h;
}


//-----------------------------------------------------------------------------
// 64 bit murmur2 mixing function. Note the last two lines are swapped
// compared with 32 bit murmur2_mix. It's not clear if this is deliberate
// in the canonical implementation, so we just leave it to help compatibility.
uint64_t
util::hash::murmur2::mix (uint64_t h, uint64_t k)
{
    static const uint64_t m = constants<uint64_t>::m;
    static const uint64_t r = constants<uint64_t>::r;

    k *= m;
    k ^= k >> r;
    k *= m;

    h ^= k;
    h *= m;

    return h;
}


///////////////////////////////////////////////////////////////////////////////
uint32_t
util::hash::murmur2::hash_32 (const void *restrict key,
                              size_t len,
                              uint32_t seed)
{
    CHECK (key);

    // setup
    static const auto m = constants<uint32_t>::m;
    uint32_t h = seed ^ uint32_t (len);

    // body
    auto cursor = reinterpret_cast<const uint32_t*> (key);
    auto last   = cursor + len / sizeof (uint32_t);
    for (; cursor < last; ++cursor)
        h = mix (h, *cursor);

    // tail
    if (len % sizeof (uint32_t)) {
        h ^= murmur::tail (cursor, len);
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
    const auto m = constants<uint64_t>::m;
    const auto r = constants<uint64_t>::r;

    uint64_t h = seed ^ (len * m);

    // body
    auto cursor = reinterpret_cast<const uint64_t*> (key);
    auto last   = cursor + len / sizeof (uint64_t);
    for (; cursor < last; ++cursor)
        h = mix (h, *cursor);

    // tail
    if (len % sizeof (uint64_t)) {
        h ^= murmur::tail (cursor, len);
        h *= m;
    }

    // finalise
    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}


