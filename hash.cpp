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

#include "hash.hpp"


uint32_t
util::wang32 (uint32_t key) {
  uint32_t c2 = 0x27d4eb2d; // a prime or an odd constant
  key = (key ^ 61) ^ (key >> 16);
  key = key + (key << 3);
  key = key ^ (key >> 4);
  key = key * c2;
  key = key ^ (key >> 15);
  return key;
}


uint64_t
util::wang64 (uint64_t key) {
  key = (~key) + (key << 21); // key = (key << 21) - key - 1;
  key = key ^ (key >> 24);
  key = (key + (key << 3)) + (key << 8); // key * 265
  key = key ^ (key >> 14);
  key = (key + (key << 2)) + (key << 4); // key * 21
  key = key ^ (key >> 28);
  key = key + (key << 31);

  return key;
}



//   32: 2^ 24 + 2^8 + 0x93
//   64: 2^ 40 + 2^8 + 0xb3
//  128: 2^ 88 + 2^8 + 0x3B
//  256: 2^168 + 2^8 + 0x63
//  512: 2^344 + 2^8 + 0x57
// 1024: 2^680 + 2^8 + 0x8D
static const uint32_t FNV_1a_32_PRIME =      16777619U;
static const uint64_t FNV_1a_64_PRIME = 1099511628211U;

// Bias is the FNV-0 hash of "chongo <Landon Curt Noll> /\\../\\"
static const uint32_t FNV_1a_32_BIAS  =           2166136261U;
static const uint64_t FNV_1a_64_BIAS  = 14695981039346656037U;


uint32_t
util::fnv1a32 (const void *_data, size_t size) {
    const uint8_t *data = static_cast<const uint8_t*> (_data);
    uint32_t result = FNV_1a_32_BIAS;

    for (size_t i = 0; i < size; ++i) {
        result ^= data[i];
        result *= FNV_1a_32_PRIME;
    }

    return result;
}


uint64_t
util::fnv1a64 (const void *_data, size_t size) {
    const uint8_t *data = static_cast<const uint8_t*> (_data);
    uint64_t result = FNV_1a_64_BIAS;

    for (size_t i = 0; i < size; ++i) {
        result ^= data[i];
        result *= FNV_1a_64_PRIME;
    }

    return result;
}
