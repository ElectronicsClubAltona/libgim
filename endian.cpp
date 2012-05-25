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

#include "endian.hpp"

#include <cstdio>
#include <type_traits>

using std::is_fundamental;
using std::is_unsigned;

#ifdef WORDS_BIGENDIAN
/* Big endian doesn't need swapping */
template <typename T>
T hton (T val) {
    static_assert (is_fundamental <T>(), "hton implementation assumes fundamental types");
    static_assert (is_unsigned    <T>(), "hton implementation does not handle signed");
    return val;
}

template <typename T>
T ntoh (T val) {
    static_assert (is_fundamental <T>(), "ntoh implementation assumes fundamental types");
    static_assert (is_unsigned    <T>(), "ntoh implementation does not handle signed");
    return val;
}

template <>  uint8_t hton ( uint8_t);
template <> uint16_t hton (uint16_t);
template <> uint32_t hton (uint32_t);
template <> uint64_t hton (uint64_t);

template <>  uint8_t ntoh ( uint8_t);
template <> uint16_t ntoh (uint16_t);
template <> uint32_t ntoh (uint32_t);
template <> uint64_t ntoh (uint64_t);

#else

static void
byte_swap (uint8_t *restrict dst, uint8_t *restrict src, size_t len) {
    for (unsigned int i = 0; i < len; ++i)
        dst[len - i - 1] = src[i];
}


template <typename T>
T
hton (T i) {
    // Unsure if this will really be sensible for non-intrinsic types, or types larger than 8 bytes.
    static_assert (is_fundamental <T>::value, "hton implementation assumes fundamental types");
    static_assert (is_unsigned    <T>::value, "hton implementation does not handle signed");

    T swapped;
    byte_swap (reinterpret_cast <uint8_t*>(&swapped),
               reinterpret_cast <uint8_t*>(&i),
               sizeof (T));
    return swapped;
}


template <typename T>
T
ntoh (T i) {
    // Unsure if this will really be sensible for non-intrinsic types, or types larger than 8 bytes.
    static_assert (is_fundamental <T>::value, "ntoh implementation assumes fundamental types");
    static_assert (is_unsigned    <T>::value, "ntoh implementation does not handle signed");

    T swapped;
    byte_swap (reinterpret_cast <uint8_t*>(&swapped),
               reinterpret_cast <uint8_t*>(&i),
               sizeof (T));
    return swapped;
}

template <> uint8_t hton (uint8_t i) { return i; }
template <> uint8_t ntoh (uint8_t i) { return i; }

template uint16_t ntoh<uint16_t> (uint16_t);
template uint32_t ntoh<uint32_t> (uint32_t);
template uint64_t ntoh<uint64_t> (uint64_t);

template uint16_t hton<uint16_t> (uint16_t);
template uint32_t hton<uint32_t> (uint32_t);
template uint64_t hton<uint64_t> (uint64_t);

#endif


