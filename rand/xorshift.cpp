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

#include "xorshift.hpp"

#include "../debug.hpp"

#include <cstdint>

using util::rand::xorshift;


///////////////////////////////////////////////////////////////////////////////
template <typename T>
xorshift<T>::xorshift (T seed):
    m_state (seed)
{
    if (!m_state)
        throw std::invalid_argument ("xorshift seed must not be zero");
}


///////////////////////////////////////////////////////////////////////////////
// setup the constants a, b, and c. we use the values from the example
// generators provided in Marsaglia's paper.
template <typename T>
struct constants;


//-----------------------------------------------------------------------------
template <>
struct constants<uint32_t>
{
    static constexpr uint32_t a = 13u;
    static constexpr uint32_t b = 17u;
    static constexpr uint32_t c =  5u;
};


//-----------------------------------------------------------------------------
template <>
struct constants<uint64_t>
{
    static constexpr uint64_t a = 13u;
    static constexpr uint64_t b =  7u;
    static constexpr uint64_t c = 17u;
};


///////////////////////////////////////////////////////////////////////////////
template <typename T>
typename xorshift<T>::result_type
xorshift<T>::operator() (void)
{
    m_state ^= m_state >> constants<T>::a;
    m_state ^= m_state << constants<T>::b;
    m_state ^= m_state >> constants<T>::c;

    CHECK_NEZ (m_state);
    return m_state;
};


///////////////////////////////////////////////////////////////////////////////
template <typename T>
typename xorshift<T>::result_type
xorshift<T>::min (void)
{
    return 1u;
}

//-----------------------------------------------------------------------------
template <typename T>
typename xorshift<T>::result_type
xorshift<T>::max (void)
{
    return std::numeric_limits<T>::max ();
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
void
xorshift<T>::discard (unsigned count)
{
    while (count--)
        (*this)();
}


///////////////////////////////////////////////////////////////////////////////
template struct util::rand::xorshift<uint32_t>;
template struct util::rand::xorshift<uint64_t>;
