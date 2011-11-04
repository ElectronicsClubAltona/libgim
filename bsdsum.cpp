/*
 * This file is part of libgim.
 *
 * Waif is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * Waif is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2010 Danny Robson <danny@blubinc.net>
 */

#include "bsdsum.hpp"


uint16_t
bsdsum (const void *restrict _data, size_t size) {
    const uint8_t *restrict data = static_cast<const uint8_t*> (data);
    uint16_t accum = 0;

    for (size_t i = 0; i < size; ++i) {
        accum  = (accum >> 1) | ((accum & 0x1) << 15);
        accum += data[i];
    }

    return accum;
}

