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
