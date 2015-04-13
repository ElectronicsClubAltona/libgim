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

#include "bsdsum.hpp"


uint16_t
bsdsum (const void *restrict _data, size_t size) {
    const uint8_t *restrict data = static_cast<const uint8_t*> (_data);
    uint16_t accum = 0;

    for (size_t i = 0; i < size; ++i) {
        accum  = (accum >> 1u) | ((accum & 0x01u) << 15u);
        accum += data[i];
    }

    return accum;
}

