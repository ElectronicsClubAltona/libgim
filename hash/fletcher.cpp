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


#include "./fletcher.hpp"

#include "../debug.hpp"


using util::hash::fletcher;


///////////////////////////////////////////////////////////////////////////////
template <typename T>
fletcher<T>::fletcher (part_t _modulus, part_t _a, part_t _b):
    m_modulus { _modulus },
    m_initial { _a, _b },
    m_state   { m_initial }
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
void
fletcher<T>::reset (void)
{
    m_state = m_initial;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
void
fletcher<T>::update (const void *restrict data, size_t size) noexcept
{
    CHECK (data);

    auto first = static_cast<const uint8_t *restrict> (data);
    update (first, first + size);
}


//-----------------------------------------------------------------------------
template <typename T>
void
fletcher<T>::update (const uint8_t *restrict first, const uint8_t *restrict last) noexcept
{
    CHECK (first);
    CHECK (last);
    CHECK_LE (first, last);

    for (auto cursor = first; cursor < last; ++cursor) {
        m_state.a = (m_state.a + *cursor  ) % m_modulus;
        m_state.b = (m_state.a + m_state.b) % m_modulus;
    }
}


//-----------------------------------------------------------------------------
template <typename T>
void
fletcher<T>::finish (void)
{ ; }


///////////////////////////////////////////////////////////////////////////////
template <typename T>
typename fletcher<T>::digest_t
fletcher<T>::digest (void) const
{
    return (m_state.b << (sizeof (part_t) * 8)) + m_state.a;
}


///////////////////////////////////////////////////////////////////////////////
template class util::hash::fletcher<uint32_t>;
