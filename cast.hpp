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


///////////////////////////////////////////////////////////////////////////////
/// Safely cast a numeric type to its (un)signed counterpart, aborting if the
/// dynamically checked result is not representable. May be optimised out if
/// NDEBUG is defined.
///
/// The signed/unsigned and unsigned/signed cases are split so we can simplify
/// the out of range tests.
///
/// The same-type case is not provided because we want to error out on
/// unnecessary casts.
template <
    typename T,
    typename U
>
std::enable_if_t<
    sizeof(T) == sizeof(U) &&
    std::is_unsigned<T>::value &&
    std::is_signed<U>::value,
    T
>
sign_cast (const U u)
{
    CHECK_GE (u, 0);
    return static_cast<T> (u);
}

//-----------------------------------------------------------------------------
template <
    typename T,
    typename U
>
std::enable_if_t<
    sizeof(T) == sizeof (U)  &&
    std::is_signed<T>::value &&
    std::is_unsigned<U>::value,
    T
>
sign_cast (const U u)
{
    CHECK_LT (u, std::numeric_limits<U>::max () / 2);
    return static_cast<T> (u);
}


///----------------------------------------------------------------------------
/// assert if the value cannot be cast loslessly from U to T, else return the
/// converted value.Note: this is only a debug-time check and is compiled out
/// in optimised builds.
template <
    typename T,
    typename U
>
std::enable_if_t<
    // either we're casting from real to integer
    !std::is_floating_point<T>::value && std::is_floating_point<U>::value
    // or we're reducing the size of the type
    || sizeof (T) < sizeof (U),
    T
>
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


///////////////////////////////////////////////////////////////////////////////
/// assert if the value is not a pointer to a subclass of T, else return the
/// converted value. Note: this is only a debug-time check and is compiled out
/// in optimised builds.
template <typename T, typename V>
T*
known_cast (V *v)
{
    CHECK (dynamic_cast<T*> (v));
    return static_cast<T*> (v);
}


//-----------------------------------------------------------------------------
template <typename T, typename V>
T&
known_cast (V &v)
{
    CHECK_NOTHROW (dynamic_cast<T> (v));
    return static_cast<T> (v);
}

#endif
