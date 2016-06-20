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

using util::hash::bsdsum;


///////////////////////////////////////////////////////////////////////////////
bsdsum::bsdsum ()
{
    reset ();
}


//-----------------------------------------------------------------------------
void
bsdsum::reset (void)
{
    m_accum = 0;
}


///////////////////////////////////////////////////////////////////////////////
void
bsdsum::update (const void *restrict data, size_t size) noexcept
{
    auto first = static_cast<const uint8_t *restrict> (data);

    update (first, first + size);
}


///////////////////////////////////////////////////////////////////////////////
void
bsdsum::update (
    const uint8_t *const restrict first,
    const uint8_t *const restrict last) noexcept
{
    CHECK (first);
    CHECK (last);
    CHECK_LE (first, last);

    for (auto cursor = first; cursor != last; ++cursor) {
        m_accum  = (m_accum >> 1u) | ((m_accum & 0x01u) << 15u);
        m_accum += *cursor;
    }
}


//-----------------------------------------------------------------------------
void
bsdsum::finish (void)
{ ; }



//-----------------------------------------------------------------------------
typename bsdsum::digest_t
bsdsum::digest (void) const
{
    return m_accum;
}
