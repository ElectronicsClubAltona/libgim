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


///////////////////////////////////////////////////////////////////////////////
// analogue of std::data for use until we get proper c++17 support
namespace util {
    template <typename T>
    auto
    data (T &t)
    { return t.data (); }

    //-------------------------------------------------------------------------
    template <typename T>
    auto
    data (const T &t)
    { return t.data (); }


    //-------------------------------------------------------------------------
    template <typename T, size_t N>
    constexpr T*
    data (T (&t)[N])
    { return &t[0]; }


    //-------------------------------------------------------------------------
    template <typename T, size_t N>
    constexpr const T*
    data (const T (&t)[N])
    { return &t[0]; }
}


///////////////////////////////////////////////////////////////////////////////
/// returns the first argument from a parameter pack which can evaluate to true.
template <class T>
T
first (T a)
{
    if (a)
        return a;

    throw std::logic_error ("no valid object");
}

//-----------------------------------------------------------------------------
template <class T, class ...Args>
T
first (T a, Args&& ...b)
{
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


    ///------------------------------------------------------------------------
    /// count the number of parameters we are given. useful for counting
    /// arguments in variadic macros (ie, sizeof... (__VA_ARGS__))
    template <typename ...T>
    constexpr size_t
    param_count (const T... t)
    {
        // prevent unused variable warnings by never forwarding recursively
        // ideally we'd use void casting, but it doesn't work for parameter
        // packs
        if (false)
            return param_count (t...);

        return sizeof... (t);
    }
}

#endif
