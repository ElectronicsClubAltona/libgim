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
 * Copyright 2010 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_FLETCHER_HPP
#define __UTIL_FLETCHER_HPP

#include "types/bits.hpp"

#include <cstdint>
#include <cstdlib>


template <
    unsigned OUTPUT,
    unsigned MODULUS,
    unsigned INITIAL_A,
    unsigned INITIAL_B
>
typename bits_type<OUTPUT>::uint
fletcher (const void *restrict _data,
          size_t size)
{
    typedef typename bits_type<OUTPUT / 2>::uint temp_t;

    auto data = static_cast<const uint8_t*> (_data);
    temp_t A = INITIAL_A, B = INITIAL_B;

    for (size_t i = 0; i < size; ++i) {
        A = (A + data[i]) % MODULUS;
        B = (A +       B) % MODULUS;
    }

    return (B << (OUTPUT / 2u)) + A;
}

#endif

