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
 * Copyright 2010 Danny Robson <danny@blubinc.net>
 */

#ifndef __UTIL_HASH_HPP
#define __UTIL_HASH_HPP

#include <cstdint>
#include <cstdlib>

// All hashes are unsuitable for cryptographic operations nnless noted.
namespace util {
    // Fast integer mixing operations by Thomas Wang.
    uint32_t  wang32 (uint32_t key);
    uint64_t  wang64 (uint64_t key);

    // Fast and general hashing using FNV-1a
    uint32_t fnv1a32 (const void *, size_t);
    uint64_t fnv1a64 (const void *, size_t);

    // General hashes for when you really just don't care about implementation
    inline  uint32_t hash (uint32_t key)    { return wang32 (key); }
    inline   int32_t hash ( int32_t key)    { return (int32_t) hash ((uint32_t)key); }

    inline  uint64_t hash (uint64_t key)    { return wang64 (key); }
    inline   int64_t hash ( int64_t key)    { return (int64_t) hash ((uint64_t)key); }

    inline uintptr_t hash (const void *key) {
        return sizeof (uintptr_t) == 32 ? wang32 (reinterpret_cast<uintptr_t> (key)) :
                                          wang64 (reinterpret_cast<uintptr_t> (key));
    }
}




#endif
