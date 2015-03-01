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
