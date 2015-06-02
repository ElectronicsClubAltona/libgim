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

#ifndef __UTIL_HASH_HPP
#define __UTIL_HASH_HPP

#include <cstdint>
#include <cstdlib>

// All hashes are unsuitable for cryptographic operations nnless noted.
namespace util {
    // Fast and general hashing using FNV-1a
    uint32_t fnv1a32 (const void *, size_t);
    uint64_t fnv1a64 (const void *, size_t);

    // General hashes for when you really just don't care about implementation
    //inline  uint32_t hash (uint32_t key)    { return wang32 (key); }
    //inline   int32_t hash ( int32_t key)    { return (int32_t) hash ((uint32_t)key); }

    //inline  uint64_t hash (uint64_t key)    { return wang64 (key); }
    //inline   int64_t hash ( int64_t key)    { return (int64_t) hash ((uint64_t)key); }

    //inline uintptr_t hash (const void *key) {
    //    return sizeof (uintptr_t) == 32 ? wang32 (reinterpret_cast<uintptr_t> (key)) :
    //                                      wang64 (reinterpret_cast<uintptr_t> (key));
    //}
}




#endif
