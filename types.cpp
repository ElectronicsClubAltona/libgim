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

#include "types.hpp"

#include <type_traits>

using namespace std;


#define do_type_to_string(T)                                                    \
template <> std::string type_to_string <T>       (void) { return #T; }          \
template <> std::string type_to_string <const T> (void) { return "const " #T; }

do_type_to_string (float)
do_type_to_string (double)

do_type_to_string (  int8_t)
do_type_to_string ( int16_t)
do_type_to_string ( int32_t)
do_type_to_string ( int64_t)

do_type_to_string ( uint8_t)
do_type_to_string (uint16_t)
do_type_to_string (uint32_t)
do_type_to_string (uint64_t)

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


fourcc
fourcc::from_chars (uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    fourcc lhs;

    lhs.data[0] = a;
    lhs.data[1] = b;
    lhs.data[2] = c;
    lhs.data[3] = d;

    return lhs;
}


fourcc
fourcc::from_str (const char data[4]) {
    fourcc lhs;

    lhs.data[0] = (uint8_t)data[0];
    lhs.data[1] = (uint8_t)data[1];
    lhs.data[2] = (uint8_t)data[2];
    lhs.data[3] = (uint8_t)data[3];

    return lhs;
}


bool
fourcc::operator== (const char rhs[4]) const {
    return data[0] == rhs[0] &&
           data[1] == rhs[1] &&
           data[2] == rhs[2] &&
           data[3] == rhs[3];
}


fourcc::operator uint32_t (void) const {
    return (uint32_t)(data[0] << 24U |
                      data[1] << 16U |
                      data[2] <<  8U |
                      data[3]);
}


ostream&
operator<< (ostream &os, fourcc f) {
    os << f.data[0] << f.data[1] << f.data[2] << f.data[3];
    return os;
}

