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
 * Copyright 2010-2014 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_ENDIAN_HPP
#define __UTIL_ENDIAN_HPP

#include "types/bits.hpp"

#include <cstdint>
#include <type_traits>

//-----------------------------------------------------------------------------
// Uses the TIFF header values. Just because. Don't rely on this.
enum class endian : uint16_t {
    BIG    = 0x4D4D,
    LITTLE = 0x4949,
};

//-----------------------------------------------------------------------------
template <typename T>
constexpr T
bswap (T);

//template <> constexpr  int8_t bswap ( int8_t v) { return v; }
//template <> constexpr int16_t bswap (int16_t v) { return __builtin_bswap16 (v); }
//template <> constexpr int32_t bswap (int32_t v) { return __builtin_bswap32 (v); }
//template <> constexpr int64_t bswap (int64_t v) { return __builtin_bswap64 (v); }

template <> constexpr  uint8_t bswap ( uint8_t v) { return v; }
template <> constexpr uint16_t bswap (uint16_t v) { return __builtin_bswap16 (v); }
template <> constexpr uint32_t bswap (uint32_t v) { return __builtin_bswap32 (v); }
template <> constexpr uint64_t bswap (uint64_t v) { return __builtin_bswap64 (v); }


//-----------------------------------------------------------------------------
template <typename T>
constexpr T
identity (T &&v) {
    return v;
}


//-----------------------------------------------------------------------------
#if defined(WORDS_BIGENDIAN)
template <typename T> constexpr T hton (T v) { return v; }
template <typename T> constexpr T ntoh (T v) { return v; }

template <typename T> constexpr T htob (T v) { return v; }
template <typename T> constexpr T htol (T v) { return bswap (v); }

template <typename T> constexpr T btoh (T v) { return v; }
template <typename T> constexpr T ltoh (T v) { return bswap (v); }
#else
template <typename T> constexpr T hton (T v) { return bswap (v); }
template <typename T> constexpr T ntoh (T v) { return bswap (v); }

template <typename T> constexpr T htob (T v) { return bswap (v); }
template <typename T> constexpr T htol (T v) { return v; }

template <typename T> constexpr T btoh (T v) { return bswap (v); }
template <typename T> constexpr T ltoh (T v) { return v; }
#endif

//-----------------------------------------------------------------------------
struct from_endian {
    from_endian (endian _endian):
        src (_endian)
    { ; }

    template <typename T>
    T operator() (const T v) const {
        static_assert (std::is_integral<T>::value || std::is_enum<T>::value,
                       "endian conversion is only defined for integrals currently");

        auto u = static_cast<
            typename std::conditional<
                std::is_signed<T>::value,
                typename sized_type<T>::sint,
                typename sized_type<T>::uint
            >::type
        > (v);

        return static_cast<T> (
            (src == endian::LITTLE) ? ltoh (u) : btoh (u)
        );
    }

    endian src;
};

#endif
