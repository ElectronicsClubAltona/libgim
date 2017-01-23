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
#if !defined(NDEBUG)
/// assert if the value cannot be cast loslessly from U to T, else return the
/// converted value.Note: this is only a debug-time check and is compiled out
/// in optimised builds.
///
/// relies on equality operators being present and behaving sanely. attempts
/// to preserve NaN values.
///
/// allows the identity cast (if only to make cross-platform porting easier)
template <typename DstT, typename SrcT>
constexpr
std::enable_if_t<
    std::is_arithmetic<SrcT>::value &&
    std::is_arithmetic<DstT>::value,
    DstT
>
trunc_cast (const SrcT src)
{
    auto dst = static_cast<DstT> (src);

    if (!util::is_nan (src)) {
        CHECK_EQ (static_cast<SrcT> (dst), src);
    } else {
        CHECK_NEQ (dst, dst);
    }

    return dst;
}

#else

// in release mode we don't give a fuck what happens. cast away all safety.
template <typename DstT, typename SrcT>
DstT
trunc_cast (const SrcT src)
{
    return static_cast<DstT> (src);
}

#endif


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
