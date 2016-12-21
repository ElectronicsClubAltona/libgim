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
 * Copyright 2010-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifdef __UTIL_RANGE_IPP
#error
#else
#define __UTIL_RANGE_IPP
#endif

#include <limits>
#include <type_traits>


//-----------------------------------------------------------------------------
template <typename T>
template <typename U>
U
util::range<T>::normalise (T val) const
{
    static_assert (std::is_floating_point<U>::value,
                   "normalise isn't implemented for integer types");

    return static_cast<U> (val - lo) /
           static_cast<U> ( hi - lo);
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
constexpr
util::range<T>
util::range<T>::unlimited (void)
{
    return {
        std::numeric_limits<T>::has_infinity ? -std::numeric_limits<T>::infinity () :
                                                std::numeric_limits<T>::lowest   (),
        std::numeric_limits<T>::has_infinity ?  std::numeric_limits<T>::infinity () :
                                                std::numeric_limits<T>::max      ()
    };
}


//-----------------------------------------------------------------------------
template <typename T>
constexpr
util::range<T>
util::range<T>::max (void)
{
    return {
        std::numeric_limits<T>::lowest (),
        std::numeric_limits<T>::max ()
    };
}


//-----------------------------------------------------------------------------
template <typename T>
constexpr
util::range<T>
util::range<T>::unit (void)
{
    return {
        T {0}, T {1}
    };
}
