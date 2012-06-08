/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_TYPES_CASTS_HPP
#define __UTIL_TYPES_CASTS_HPP

#include "../debug.hpp"

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
        CHECK_HARD (v >= 0);
        return static_cast<T> (v);
    }


    template <typename T, typename V>
    T
    _sign_cast (const typename std::enable_if<sizeof(T) == sizeof(V)   &&
                                             std::is_signed<T>::value &&
                                             std::is_unsigned<V>::value, V>::type v)
    {
        CHECK_HARD (v < std::numeric_limits<V>::max () / 2);
        return static_cast<T> (v);
    }
}

/// Safely cast a numeric type to its (un)signed counterpart, aborting if the
/// dynamically checked result is not representable. May be optimised out if
/// NDEBUG is defined.
template <typename T, typename V>
T
sign_cast (const V v)
    { return detail::_sign_cast<T,V>(v); }


//-----------------------------------------------------------------------------
namespace detail {
    // Same sign, no possibility of truncation with larger target type
    template <typename T, typename V>
    T
    _trunc_cast (const typename std::enable_if<sizeof (T) >= sizeof (V) &&
                                               std::is_signed<T>::value == std::is_signed<V>::value, V>::type v)
        { return v; }


    template <typename T, typename V>
    T
    _trunc_cast (const typename std::enable_if<sizeof (T) < sizeof (V) &&
                                               std::is_signed<T>::value == std::is_signed<V>::value, V>::type v) {
        CHECK_HARD (v <= std::numeric_limits<T>::max ());
        checK_hard (v >= std::numeric_limits<T>::min ());

        return static_cast<T> (v);
    }
}


template <typename T, typename V>
T
trunc_cast (V v)
    { return detail::_trunc_cast<T, V> (v); }


//-----------------------------------------------------------------------------
template <typename T, typename V>
T
size_cast (const V v) {
    CHECK_HARD (std::numeric_limits<T>::min () <= v);
    CHECK_HARD (std::numeric_limits<T>::max () >= v);
    
    return static_cast<T> (v);
}

#endif
