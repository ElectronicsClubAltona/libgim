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

#ifndef __UTIL_HASH_SHA1_HPP
#define __UTIL_HASH_SHA1_HPP

#include <cstdint>
#include <cstdlib>

#include <array>


//-----------------------------------------------------------------------------
namespace util { namespace hash {
    class SHA1 {
    public:
        typedef std::array<uint8_t,20> digest_t;

    public:
        SHA1();

        void update (const uint8_t *, size_t);
        void finish (void);
        digest_t digest (void) const;
        void reset  (void);

        enum state_t {
            READY,
            FINISHED
        };

    protected:
        void process (void);

        state_t state;

        uint64_t total;
        uint32_t H[5];

        union {
            uint8_t  c[16*4+64*4];
            uint32_t W[16  +64  ];
        };
    };
} }

#endif
