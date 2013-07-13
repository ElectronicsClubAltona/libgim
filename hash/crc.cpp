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

#include "crc.hpp"

#include "debug.hpp"
#include "endian.hpp"

uint32_t
crc32 (const void *restrict, size_t) {
    not_implemented ();

    /*
    const uint8_t *restrict data = static_cast<const uint8_t*> (_data);
    static const uint32_t POLYNOMIAL = hton (static_cast<uint32_t>(0x04C11DB7));

    uint64_t bits = 0;
    unsigned int i = 0;

    if (size == 0)
        return POLYNOMIAL;

    switch (size) {
        default:    bits |= static_cast<uint64_t>(data[3]) << 32U;
        case 3:     bits |= static_cast<uint64_t>(data[2]) << 40U;
        case 2:     bits |= static_cast<uint64_t>(data[1]) << 48U;
        case 1:     bits |= static_cast<uint64_t>(data[0]) << 56U;
    }
    
    for (size_t i = 0; i < size; ++i) {
        for (unsigned j = 0; j < 32; ++j) {
            bool mix = bits 0x7000000000000000ULL;
            bits <<= 1;

            if (mix) 
                bits ^= POLYNOMIAL << 32;
        }


    }
    */
}
