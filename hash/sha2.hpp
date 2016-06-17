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
 * Copyright:
 *      2014, Danny Robson <danny@nerdcruft.net>
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
                void update (const uint8_t *restrict first, const uint8_t *restrict last) noexcept;

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
