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

#include "lcg.hpp"

using util::rand::lcg;


///////////////////////////////////////////////////////////////////////////////
template <typename T>
static constexpr
bool is_coprime (T M, T C)
{
    if (M == 0)
        return true;
    if (util::gcd (M, C) == 1u)
        return true;
    return false;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T, T M, T A, T C>
lcg<T,M,A,C>::lcg (T seed):
    m_x (seed)
{
    // ensure this assertion isn't in a header, it could be pretty expensive
    // to evaluate often.
    static_assert (is_coprime (M, C),
                   "multiplier and increment must be coprime");
}


///////////////////////////////////////////////////////////////////////////////
template <typename T, T M, T A, T C>
T
lcg<T,M,A,C>::operator() (void)
{
    m_x = (A * m_x + C);
    if (M != 0)
        m_x %= M;
    return m_x;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T, T M, T A, T C>
typename lcg<T,M,A,C>::result_type
lcg<T,M,A,C>::min (void)
{
    return std::numeric_limits<result_type>::min ();
}


//-----------------------------------------------------------------------------
template <typename T, T M, T A, T C>
typename lcg<T,M,A,C>::result_type
lcg<T,M,A,C>::max (void)
{
    return std::numeric_limits<result_type>::max ();
}


///////////////////////////////////////////////////////////////////////////////
template <typename T, T M, T A, T C>
void
lcg<T,M,A,C>::discard (unsigned count)
{
    while (count--)
        (*this)();
}


///////////////////////////////////////////////////////////////////////////////
template struct util::rand::lcg<uint32_t, util::pow2(31), 1103515245, 12345>;
template struct util::rand::lcg<uint64_t, 0ul, 6364136223846793005ul, 1ul>;
