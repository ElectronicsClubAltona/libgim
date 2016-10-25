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
 * Copyright 2011-2016 Danny Robson <danny@nerdcruft.net>
 */

#include "./point.hpp"

#include "./debug.hpp"

#include <cstdlib>

using util::point;


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
util::vector<S,T>
util::point<S,T>::to (point<S,T> rhs) const
{
    util::vector<S,T> out;
    for (size_t i = 0; i < S; ++i)
        out.data[i] = rhs.data[i] - this->data[i];
    return out;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::vector<S,T>
util::point<S,T>::from (point<S,T> rhs) const
{
    util::vector<S,T> out;
    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] - rhs.data[i];
    return out;
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
void
util::point<S,T>::sanity (void) const
{
    CHECK (std::all_of (this->begin (),
                        this->end (),
                        [] (auto i) { return !std::isnan (i); }));
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
const util::point<S,T> util::point<S,T>::ORIGIN (T {0});

//-----------------------------------------------------------------------------
#define INSTANTIATE_S_T(S,T)        \
template struct util::point<S,T>;

#define INSTANTIATE(T)  \
INSTANTIATE_S_T(1,T)    \
INSTANTIATE_S_T(2,T)    \
INSTANTIATE_S_T(3,T)    \
INSTANTIATE_S_T(4,T)


INSTANTIATE(int16_t)
INSTANTIATE(int32_t)
INSTANTIATE(int64_t)

INSTANTIATE(uint16_t)
INSTANTIATE(uint32_t)
INSTANTIATE(uint64_t)

INSTANTIATE(float)
INSTANTIATE(double)
