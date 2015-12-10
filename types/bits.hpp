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

#ifndef __UTIL_TYPES_BITS_HPP
#define __UTIL_TYPES_BITS_HPP

#include <cstdint>
#include <cstddef>

template <size_t BITS>
struct bits_type;


template <> struct bits_type< 8u> {
    static const bool has_floating = false;

    typedef    uint8_t uint;
    typedef     int8_t sint;
    typedef    uint8_t floating;
};


template <> struct bits_type<16u> {
    static const bool has_floating = false;

    typedef   uint16_t uint;
    typedef    int16_t sint;
    typedef   uint16_t floating;
};


template <> struct bits_type<32u> {
    static const bool has_floating = true;

    typedef uint32_t uint;
    typedef  int32_t sint;
    typedef    float floating;
};


template <> struct bits_type<64u> {
    static const bool has_floating = true;

    typedef uint64_t uint;
    typedef  int64_t sint;
    typedef   double floating;
};


template <typename T>
struct sized_type : public bits_type<sizeof(T) * 8u>
{ };

#endif
