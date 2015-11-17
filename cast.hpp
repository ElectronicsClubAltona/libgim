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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_CAST_HPP
#define __UTIL_CAST_HPP

#include "./debug.hpp"

#include <type_traits>
#include <limits>


//-----------------------------------------------------------------------------
namespace detail {
    template <typename T, typename V>
    T
    _sign_cast (const typename std::enable_if<sizeof(T) == sizeof(V)     &&
                                              std::is_unsigned<T>::value &&
                                              std::is_signed<V>::value, V>::type v)
    {
        CHECK_GE (v, 0);
        return static_cast<T> (v);
    }


    template <typename T, typename V>
    T
    _sign_cast (const typename std::enable_if<sizeof(T) == sizeof(V)  &&
                                             std::is_signed<T>::value &&
                                             std::is_unsigned<V>::value, V>::type v)
    {
        CHECK_LT (v, std::numeric_limits<V>::max () / 2);
        return static_cast<T> (v);
    }


    template <typename T, typename V>
    T
    _sign_cast (const typename std::enable_if<std::is_same<T, V>::value, V>::type v)
        { return v; }
}


/// Safely cast a numeric type to its (un)signed counterpart, aborting if the
/// dynamically checked result is not representable. May be optimised out if
/// NDEBUG is defined.
template <typename T, typename V>
T
sign_cast (const V v)
    { return detail::_sign_cast<T,V>(v); }


///----------------------------------------------------------------------------
/// assert if the value cannot be cast loslessly from U to T, else return the
/// converted value.Note: this is only a debug-time check and is compiled out
/// in optimised builds.

template <typename T, typename U>
T
trunc_cast (U u)
{
    auto t = static_cast<T> (u);

    // assume testing round-trip error is the same as half-trip error. this
    // makes satisfying the type system (for the debug checks) a lot easier,
    // but seems like it might bite us later...
    if (u == u)
        CHECK_EQ  (static_cast<U> (t), u);
    else
        CHECK_NEQ (t, t);

    return t;
}


///----------------------------------------------------------------------------
/// assert if the value is not a pointer to a subclass of T, else return the
/// converted value. Note: this is only a debug-time check and is compiled out
/// in optimised builds.
template <typename T, typename V>
T*
known_cast (V *v) {
    CHECK (dynamic_cast<T*> (v));
    return static_cast<T*> (v);
}


//-----------------------------------------------------------------------------
template <typename T, typename V>
T&
known_cast (V &v) {
    CHECK_NOTHROW (dynamic_cast<T> (v));
    return static_cast<T> (v);
}

#endif
