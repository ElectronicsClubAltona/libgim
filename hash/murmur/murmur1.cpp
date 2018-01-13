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

using util::hash::murmur1;


///////////////////////////////////////////////////////////////////////////////
uint32_t
murmur1::operator() (util::view<const uint8_t*> data) const noexcept
{
    static const uint32_t m = 0xc6a4a793;
    uint32_t h = m_seed ^ ((data.size () & 0xffffffff) * m);

    // mix the body
    auto cursor = reinterpret_cast<const uint32_t*> (data.data ());
    auto last   = cursor + data.size () / sizeof (uint32_t);
    for (; cursor < last; ++cursor)
        h = mix (h, *cursor);

    // mix the tail
    if (data.size () % sizeof (uint32_t))
        h = mix (h, murmur::tail<uint32_t> (reinterpret_cast<const uint8_t*> (cursor), data.size ()));

    // finalise
    h *= m; h ^= h >> 10;
    h *= m; h ^= h >> 17;

    return h;
}

