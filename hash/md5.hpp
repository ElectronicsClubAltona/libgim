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
 * Copyright 2013 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_MD5_HPP
#define __UTIL_MD5_HPP

#include <array>
#include <cstdint>
#include <cstdlib>

namespace util {
    namespace hash {
        class MD5 {
            public:
                typedef std::array<uint8_t,16> digest_t;

            public:
                MD5();

                void update (const void    *data, size_t len);
                void update (const uint8_t *data, size_t len);

                void finish (void);
                digest_t digest (void) const;
                void reset (void);

            private:
                void transform (void);

                uint64_t m_total;
                std::array<uint32_t, 4> ABCD;

                union {
                    uint32_t X [16];
                    uint8_t  Xb[64];
                };
        };
    }

    typedef uint8_t md5_t;
    md5_t md5 (const void *restrict data, size_t len);
}

#endif
