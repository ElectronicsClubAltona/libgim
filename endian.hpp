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
 * Copyright 2010-2014 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_ENDIAN_HPP
#define __UTIL_ENDIAN_HPP

#include "types/bits.hpp"

#include <cstdint>
#include <type_traits>

namespace util {
    //-------------------------------------------------------------------------
    // Uses the TIFF header values. Just because. Don't rely on this.
    enum class endian : uint16_t {
        BIG    = 0x4D4D,
        LITTLE = 0x4949,
    };

    //-------------------------------------------------------------------------
    template <typename T>
    constexpr T
    bswap (T);

    // CXX: Update using "if constexpr" when available. It doesn't seem to be
    // worth the dicking about to use enable_if_t to differentiate the
    // signed/unsigned cases.
    #define SIGNED_BSWAP(S)                 \
    template <>                             \
    constexpr                               \
    int##S##_t                              \
    bswap (int##S##_t v) {                  \
        const union {                       \
             int##S##_t s;                  \
            uint##S##_t u;                  \
        } vu = { v };                       \
                                            \
        return __builtin_bswap##S (vu.u);   \
    }

    SIGNED_BSWAP(16)
    SIGNED_BSWAP(32)
    SIGNED_BSWAP(64)

    template <> constexpr  int8_t bswap ( int8_t v) { return v; }
    template <> constexpr uint8_t bswap (uint8_t v) { return v; }

    template <> constexpr uint16_t bswap (uint16_t v) { return __builtin_bswap16 (v); }
    template <> constexpr uint32_t bswap (uint32_t v) { return __builtin_bswap32 (v); }
    template <> constexpr uint64_t bswap (uint64_t v) { return __builtin_bswap64 (v); }


    //-------------------------------------------------------------------------
    template <typename T>
    constexpr T
    identity (T &&v) {
        return v;
    }


    //-------------------------------------------------------------------------
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

    //-------------------------------------------------------------------------
    struct from_endian {
        explicit from_endian (endian _endian):
            src (_endian)
        { ; }

        template <typename T>
        T operator() (const T v) const {
            static_assert (std::is_integral<T>::value || std::is_enum<T>::value,
                           "endian conversion is only defined for integrals currently");

            union {
                typename sized_type<T>::sint sint;
                typename sized_type<T>::uint uint;
            };

            if (std::is_signed<T>::value)
                sint = v;
            else
                uint = v;

            uint = (src == endian::LITTLE) ? ltoh (uint) : btoh (uint);

            if (std::is_signed<T>::value)
                return T (sint);
            else
                return T (uint);
        }

        endian src;
    };
}
#endif
