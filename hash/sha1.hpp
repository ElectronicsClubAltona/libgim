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
        static const size_t BLOCK_SIZE  = 64;
        static const size_t DIGEST_SIZE = 20;

    public:
        SHA1();

        void update (const uint8_t *, size_t);
        void update (const uint8_t *restrict first, const uint8_t *restrict last) noexcept;

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
