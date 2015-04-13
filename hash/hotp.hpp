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

#ifndef __UTIL_HASH_HOTP_HPP
#define __UTIL_HASH_HOTP_HPP

#include "hmac.hpp"
#include "sha1.hpp"

#include <cstdint>

namespace util { namespace hash {
    /// HMAC one-time password (RFC 4226)
    class HOTP {
    public:
        HOTP (const char *key, uint64_t counter);
        HOTP (const void *key, size_t len, uint64_t counter);

        unsigned value (void);
        uint64_t counter (void) const;

    private:
        uint32_t truncate (SHA1::digest_t);

        uint64_t m_counter;
        HMAC<SHA1> m_hash;
    };
} }

#endif
