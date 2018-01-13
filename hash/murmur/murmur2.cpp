
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

using util::hash::murmur2;


///////////////////////////////////////////////////////////////////////////////
static uint32_t
hash_32 (const void *restrict key,
                           size_t len,
                           uint32_t seed)
{
    // setup
    constexpr auto m = util::hash::detail::murmur2::constants<uint32_t>::m;
    uint32_t h = seed ^ (len & 0xffffffff);

    // body
    auto cursor = reinterpret_cast<const uint32_t*> (key);
    auto last   = cursor + len / sizeof (uint32_t);
    for (; cursor < last; ++cursor)
        h = util::hash::murmur2<uint32_t>::mix (h, *cursor);

    // tail
    if (len % sizeof (uint32_t)) {
        h ^= util::hash::murmur::tail<uint32_t> (reinterpret_cast<const uint8_t*> (cursor), len);
        h *= m;
    }

    // finalise
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}


//-----------------------------------------------------------------------------
static uint64_t
hash_64 (const void *restrict key,
                           size_t len,
                           uint64_t seed)
{
    // setup
    constexpr auto m = util::hash::detail::murmur2::constants<uint64_t>::m;
    constexpr auto r = util::hash::detail::murmur2::constants<uint64_t>::r;

    uint64_t h = seed ^ (len * m);

    // body
    auto cursor = reinterpret_cast<const uint64_t*> (key);
    auto last   = cursor + len / sizeof (uint64_t);
    for (; cursor < last; ++cursor)
        h = util::hash::murmur2<uint64_t>::mix (h, *cursor);

    // tail
    if (len % sizeof (uint64_t)) {
        h ^= util::hash::murmur::tail<uint64_t> (reinterpret_cast<const uint8_t*> (cursor), len);
        h *= m;
    }

    // finalise
    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}


///////////////////////////////////////////////////////////////////////////////
template <typename DigestT>
typename murmur2<DigestT>::digest_t
murmur2<DigestT>::operator() (util::view<const uint8_t*> data) const noexcept
{
    static_assert (std::is_same_v<DigestT,uint32_t> || std::is_same_v<DigestT,uint64_t>);

    if constexpr (std::is_same_v<DigestT,uint32_t>)
        return hash_32 (data.data (), data.size (), m_seed);

    if constexpr (std::is_same_v<DigestT,uint64_t>)
        return hash_64 (data.data (), data.size (), m_seed);

    unreachable ();
}


///////////////////////////////////////////////////////////////////////////////
template class util::hash::murmur2<uint32_t>;
template class util::hash::murmur2<uint64_t>;
