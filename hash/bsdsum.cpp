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

