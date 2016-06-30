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
 * Copyright 2016 Danny Robson <danny@nerdcruft.net>
 */


#include "./mwc64x.hpp"

#include "../debug.hpp"

using util::rand::mwc64x;


///////////////////////////////////////////////////////////////////////////////
mwc64x::mwc64x (seed_type _seed):
    m_state (_seed)
{
    CHECK_NEZ (m_state);
}


///////////////////////////////////////////////////////////////////////////////
mwc64x::value_type
mwc64x::operator() (void)
{
    CHECK_NEZ (m_state);

    uint32_t c = m_state >> 32u;
    uint32_t x = m_state & 0xFFFFFFFFu;

    m_state = x * 4294883355ULL + c;

    return x ^ c;
}
