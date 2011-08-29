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
 * Copyright 2011 Danny Robson <danny@blubinc.net>
 */

#ifndef __TYPES_HPP
#define __TYPES_HPP

#include "annotations.hpp"
#include "debug.hpp"
#include "enable_if.hpp"

#include <cstdint>
#include <string>
#include <limits>
#include <memory>

template <int BITS>
struct bits_type;


template <> struct bits_type< 8> {
    static const bool has_floating = false;

    typedef    uint8_t uint;
    typedef     int8_t sint;
    typedef    uint8_t floating;
};


template <> struct bits_type<16> {
    static const bool has_floating = false;

    typedef   uint16_t uint;
    typedef    int16_t sint;
    typedef   uint16_t floating;
};


template <> struct bits_type<32> {
    static const bool has_floating = true;

    typedef uint32_t uint;
    typedef  int32_t sint;
    typedef    float floating;
};


template <> struct bits_type<64> {
    static const bool has_floating = true;

    typedef uint64_t uint;
    typedef  int64_t sint;
    typedef   double floating;
};


template <typename T>
struct sized_type : public bits_type<sizeof(T) * 8>
{ };


template <typename T>
std::string type_to_string (void);


namespace detail {
    template <typename T, typename V>
    T
    _sign_cast (const typename enable_if<sizeof(T) == sizeof(V)     &&
                                         std::is_unsigned<T>::value &&
                                         std::is_signed<V>::value, V>::type v)
    {
        check_hard (v >= 0);
        return static_cast<T> (v);
    }


    template <typename T, typename V>
    T
    _sign_cast (const typename enable_if<sizeof(T) == sizeof(V)   &&
                                         std::is_signed<T>::value &&
                                         std::is_unsigned<V>::value, V>::type v)
    {
        check_hard (v < std::numeric_limits<V>::max () / 2);
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


namespace detail {
    // Same sign, no possibility of truncation with larger target type
    template <typename T, typename V>
    T
    _trunc_cast (const typename enable_if<sizeof (T) >= sizeof (V) &&
                                          std::is_signed<T>::value == std::is_signed<V>::value, V>::type v)
        { return v; }


    template <typename T, typename V>
    T
    _trunc_cast (const typename enable_if<sizeof (T) < sizeof (V) &&
                                          std::is_signed<T>::value == std::is_signed<V>::value, V>::type v) {
        check_hard (v <= std::numeric_limits<T>::max ());
        checK_hard (v >= std::numeric_limits<T>::min ());

        return static_cast<T> (v);
    }
}


template <typename T, typename V>
T
trunc_cast (V v)
    { return detail::_trunc_cast<T, V> (v); }


/// Returns the number of elements of a statically allocated array
template <typename T, size_t N>  
size_t elems(T (&)[N])
    { return N; }


template <typename T, typename ...Args>
std::unique_ptr<T>
make_unique (const Args &...args)
    { return std::unique_ptr<T> (new T (args...)); }


struct fourcc {
    uint8_t data[4];

    static fourcc from_str(const char[4]);
    static fourcc from_chars(uint8_t, uint8_t, uint8_t, uint8_t);

    bool operator== (const char[4]) const;
    operator uint32_t (void) const;
};

std::ostream& operator<< (std::ostream&, fourcc);


inline double sign (double x) { return x >= 0 ? 1.0 : -1.0; }
inline float  sign (float  x) { return x >= 0 ? 1.0 : -1.0; }
inline int    sign (int    x) { return x >= 0 ? 1   : -1  ; }


#endif // __TYPES_HPP
