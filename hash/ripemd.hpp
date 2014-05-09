/* 
 * This file is part of libgim
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2014 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_HASH_RIPEMD_HPP
#define __UTIL_HASH_RIPEMD_HPP

#include <array>

namespace util {
    namespace hash {
        class RIPEMD {
            public:
                typedef std::array<uint8_t,20> digest_t;

            public:
                RIPEMD();

                void update (const uint8_t*, size_t);
                digest_t digest (void) const;
                void finish (void);
                void reset (void);

            protected:
                void transform ();

                bool m_finished;

                uint32_t m_state[5];
                uint64_t m_length;

                struct {
                    union {
                        uint32_t d32[16];
                        uint8_t  d08[64];
                    };
                    size_t size;
                } m_buffer;
        };
    }
}

#endif
