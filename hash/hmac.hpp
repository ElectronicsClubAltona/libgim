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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_HASH_HMAC_HPP
#define __UTIL_HASH_HMAC_HPP

#include <array>
#include <cstdint>
#include <cstdlib>

namespace util { namespace hash {
    template <class T>
    /// RFC 2104 key-hashing for message authentication
    class HMAC {
    public:
        using digest_t = typename T::digest_t;

        HMAC (const uint8_t *key, size_t);

        void update (const void *restrict, size_t);
        void finish (void);
        void reset (void);

        digest_t digest (void);

    private:
        std::array<uint8_t,T::BLOCK_SIZE> m_ikey;
        std::array<uint8_t,T::BLOCK_SIZE> m_okey;

        T m_hash;
    };
} }

#endif
