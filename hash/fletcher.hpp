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

#ifndef __UTIL_FLETCHER_HPP
#define __UTIL_FLETCHER_HPP

#include "types/bits.hpp"

#include <cstdint>
#include <cstdlib>


template <unsigned BITS, unsigned MODULUS, unsigned INITIAL_A, unsigned INITIAL_B>
typename bits_type<BITS>::uint
fletcher (const void *restrict _data, size_t size) {
    const uint8_t *restrict data = static_cast<const uint8_t*> (_data);
    typename bits_type<BITS>::uint A = INITIAL_A,
                                   B = INITIAL_B;

    for (size_t i = 0; i < size; ++i) {
        A = (A + data[i]) % MODULUS;
        B = (A +       B) % MODULUS;
    }

    return (B << (BITS / 2)) | A;
}

#endif

