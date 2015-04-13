/*
 * This file is part of libgim
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2014 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_HASH_SHA2_HPP
#define __UTIL_HASH_SHA2_HPP

#include <array>
#include <cstdint>

namespace util {
    namespace hash {
        class SHA256 {
            public:
                typedef std::array<uint8_t,32> digest_t;

            public:
                SHA256();

                void update (const uint8_t *, size_t);
                void finish (void);
                digest_t digest (void) const;

                void reset (void);

            private:
                void process (void);

                uint64_t m_total;
                std::array<uint32_t, 8> H;

                union {
                    std::array<uint32_t, 16> M;
                    std::array<uint8_t,  64> C;
                };
        };
    }
}

#endif
