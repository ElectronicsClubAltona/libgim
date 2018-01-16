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

#include "debug.hpp"

#include <type_traits>
#include <limits>


namespace util::cast {
    ///////////////////////////////////////////////////////////////////////////
    /// Safely cast a numeric type to its (un)signed counterpart, aborting if
    /// the dynamically checked result is not representable. May be optimised
    /// out if NDEBUG is defined.
    ///
    /// The signed/unsigned and unsigned/signed cases are split so we can
    /// simplify the out of range tests.
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
    sign (const U u)
    {
        CHECK_GE (u, 0);
        return static_cast<T> (u);
    }


    //-------------------------------------------------------------------------
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
    sign (const U u)
    {
        CHECK_LT (u, std::numeric_limits<U>::max () / 2);
        return static_cast<T> (u);
    }


    ///////////////////////////////////////////////////////////////////////////
    // cast to a smaller type and check that both values are still equal.
    //
    // checks will be compiled out if NDEBUG is defined.
    template <typename NarrowT, typename WideT>
    constexpr NarrowT
    narrow (const WideT &val)
    {
        static_assert (sizeof (NarrowT) <= sizeof (WideT));
        static_assert (std::is_signed_v<NarrowT> == std::is_signed_v<WideT>);

#ifndef NDEBUG
        auto narrow = static_cast<NarrowT> (val);
        CHECK_EQ (narrow, val);
        return narrow;
#else
        return static_cast<NarrowT> (val);
#endif
    }


    ///////////////////////////////////////////////////////////////////////////
    // cast between types checking that equality holds with the result
    //
    // checks will be compiled out if NDEBUG is defined.
    template <typename DstT, typename SrcT>
    constexpr DstT
    lossless (const SrcT &src)
    {
#ifndef NDEBUG
        auto dst = static_cast<DstT> (src);

        if constexpr (std::is_floating_point_v<SrcT>) {
            if (util::is_nan (src)) {
                // NaNs must remaing as NaN's. They're important.
                CHECK (std::is_floating_point_v<DstT>);
                CHECK (std::isnan (dst));
            }
        }

        // Cast dst back to src to check round-trip conversion
        // is lossless.
        CHECK_EQ (static_cast<SrcT> (dst), src);
        return dst;

#else
        return static_cast<DstT> (src);
#endif
    }


    ///////////////////////////////////////////////////////////////////////////
    /// assert if the value is not a pointer to a subclass of T, else return
    /// the converted value. Note: this is only a debug-time check and is
    /// compiled out in optimised builds.
    template <typename T, typename V>
    T*
    known (V *v)
    {
        CHECK (dynamic_cast<T*> (v));
        return static_cast<T*> (v);
    }


    //-------------------------------------------------------------------------
    template <typename T, typename V>
    T&
    known (V &v)
    {
        CHECK_NOTHROW (dynamic_cast<T> (v));
        return static_cast<T> (v);
    }
};

#endif
