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
 * Copyright 2011-2014 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_TYPES_HPP
#define __UTIL_TYPES_HPP

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <stdexcept>

//-----------------------------------------------------------------------------
/// Returns the number of elements of a statically allocated array
template <typename T, size_t N> 
constexpr size_t
elems (const T (&)[N])
    { return N; }


template <typename T>
constexpr typename T::size_type
elems (const T &t)
    { return t.size (); }


//-----------------------------------------------------------------------------
template <class T>
T
first (T a) {
    if (a)
        return a;

    throw std::logic_error ("no valid object");
}

template <class T, class ...Args>
T
first (T a, Args&& ...b) {
    if (a)
        return a;

    return first (std::forward<Args>(b)...);
}


namespace util {
    ///------------------------------------------------------------------------
    /// represents a type as a POD struct (but is statically recoverable via
    /// the 'type' member).
    template <typename T>
    struct type_tag
    {
        typedef T type;
    };
}

#endif
