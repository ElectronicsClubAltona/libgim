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
 * Copyright 2010 Danny Robson <danny@nerdcruft.net>
 */

#include "bsdsum.hpp"

#include "../debug.hpp"


///////////////////////////////////////////////////////////////////////////////
uint16_t
util::hash::bsdsum (
    const uint8_t *const restrict first,
    const uint8_t *const restrict last
) noexcept {
    CHECK_LE (first, last);

    uint16_t accum = 0;

    for (auto cursor = first; cursor != last; ++cursor) {
        accum  = (accum >> 1u) | ((accum & 0x01u) << 15u);
        accum += *cursor;
    }

    return accum;
}


///////////////////////////////////////////////////////////////////////////////
uint16_t
util::hash::bsdsum (const void *restrict data, size_t size) noexcept
{
    return bsdsum (
        static_cast<const uint8_t*> (data),
        static_cast<const uint8_t*> (data) + size
    );
}

