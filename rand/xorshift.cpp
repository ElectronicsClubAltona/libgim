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
 * Copyright 2015-2016 Danny Robson <danny@nerdcruft.net>
 */

#include "./xorshift.hpp"

#include "../debug.hpp"

#include <cstdint>

using util::rand::xorshift;


///////////////////////////////////////////////////////////////////////////////
template <typename T>
xorshift<T>::xorshift (T seed):
    m_state (seed)
{
    // state must not be zero, or the period will be zero.
    CHECK_NEZ (m_state);
}


///////////////////////////////////////////////////////////////////////////////
namespace util { namespace rand {
    template <>
    uint32_t
    xorshift<uint32_t>::operator() (void)
    {
        m_state ^= m_state << 13u;
        m_state ^= m_state >> 17u;
        m_state ^= m_state <<  5u;

        return m_state;
    }


    //-----------------------------------------------------------------------------
    template <>
    uint64_t
    xorshift<uint64_t>::operator() (void)
    {
        m_state ^= m_state << 13u;
        m_state ^= m_state >>  7u;
        m_state ^= m_state << 17u;

        return m_state;
    }
} }


///////////////////////////////////////////////////////////////////////////////
template struct util::rand::xorshift<uint32_t>;
template struct util::rand::xorshift<uint64_t>;
