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
 * Copyright 2010 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_ENDIAN_HPP
#define __UTIL_ENDIAN_HPP

#include "platform.hpp"

#include <cstdint>


enum {
    ENDIAN_BIG,
    ENDIAN_LITTLE
};


constexpr  uint8_t hton (uint8_t h) { return h; }
constexpr  uint8_t ntoh (uint8_t n) { return n; }


constexpr uint16_t hton (uint16_t h) {
    return ((h & 0xFF00) >> 8) |
           ((h & 0x00FF) << 8);
}

constexpr uint16_t ntoh (uint16_t n)
    { return hton (n); }


constexpr uint32_t hton (uint32_t h) {
#if defined(COMPILER_GCC)
    return __builtin_bswap32 (h);
#else
    return (h & 0xFF000000U) >> 24 |
           (h & 0x00FF0000U) >>  8 |
           (h & 0x0000FF00U) <<  8 |
           (h & 0x000000FFU) << 24;
#endif
}


constexpr uint32_t ntoh (uint32_t n)
    { return hton (n); }


constexpr uint64_t hton (uint64_t h) {
#if defined(COMPILER_GCC)
    return __builtin_bswap64 (h);
#else
    return (h & 0xFF00000000000000UL) >> 56 |
           (h & 0x00FF000000000000UL) >> 40 |
           (h & 0x0000FF0000000000UL) >> 24 |
           (h & 0x000000FF00000000UL) >>  8 |
           (h & 0x00000000FF000000UL) <<  8 |
           (h & 0x0000000000FF0000UL) << 24 |
           (h & 0x000000000000FF00UL) << 40 |
           (h & 0x00000000000000FFUL) << 56;
#endif
}


constexpr uint64_t ntoh (uint64_t n)
    { return hton (n); }

#endif
