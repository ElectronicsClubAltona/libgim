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

#include <cstdint>


//-----------------------------------------------------------------------------
template <typename T>
constexpr T
bswap (T);

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


#endif
