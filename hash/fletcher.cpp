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


#include "fletcher.hpp"

#include "../debug.hpp"


using util::hash::fletcher;


///////////////////////////////////////////////////////////////////////////////
template <typename T>
fletcher<T>::fletcher (part_t _modulus, part_t _a, part_t _b):
    m_modulus { _modulus },
    m_initial { _a, _b }
{ ; }



///////////////////////////////////////////////////////////////////////////////
template <typename DigestT>
typename fletcher<DigestT>::digest_t
fletcher<DigestT>::operator() (util::view<const std::uint8_t*> data) const noexcept
{
    state_t accum = m_initial;

    for (const auto i: data) {
        accum.a = (accum.a +       i) % m_modulus;
        accum.b = (accum.a + accum.b) % m_modulus;
    }

    return accum.b << (sizeof(part_t) * 8u) | accum.a;
}


///////////////////////////////////////////////////////////////////////////////
template class util::hash::fletcher<uint32_t>;
