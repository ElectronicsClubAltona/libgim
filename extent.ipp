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


#ifdef __UTIL_EXTENT_IPP
#error
#endif

#define __UTIL_EXTENT_IPP

#include <algorithm>


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
constexpr
T
util::extent<S,T>::diameter (void) const
{
    return static_cast<T> (
        std::sqrt (
            std::accumulate (std::begin (this->data),
                             std::end   (this->data),
                             T {0},
                             [] (auto a, auto b) { return a + b * b; })
        )
    );
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
constexpr
T
util::extent<S,T>::area (void) const
{
    return std::accumulate (std::begin (this->data),
                            std::end   (this->data),
                            T {1},
                            std::multiplies<T> ());
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
template <typename U>
constexpr
U
util::extent<S,T>::aspect (void) const
{
    return static_cast<U> (this->w) / this->h;
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
template <typename U>
bool
util::extent<S,T>::includes (point<S,U> p) const
{
    for (size_t i = 0; i < S; ++i)
        if (p[i] < 0 || static_cast<T> (p[i]) >= this->data[i])
            return false;
    return true;
}
