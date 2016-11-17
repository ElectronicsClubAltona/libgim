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
 * Copyright 2016, Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_HASH_FWD_HPP
#define __UTIL_HASH_FWD_HPP

// The single function call hashes are light weight enough to include
// directly, and are unlikely to change given their inherent
// lightweight nature.

#include "./fasthash.hpp"
#include "./fnv1a.hpp"
#include "./murmur.hpp"


// Forward declerations of class based hashes
namespace util { namespace hash {
    template <class T> class HMAC;

    // checksums
    class adler32;
    class bsdsum;
    class fletcher;
    class crc32;

    class xxhash;

    // hash adapters
    class PBKDF1;
    class PBKDF2;

    // cryptographic hashes
    class MD2;
    class MD4;
    class MD5;
    class RIPEMD;
    class SHA1;
    class SHA256;
} }

#endif
