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

#ifndef __UTIL_HASH_RIPEMD_HPP
#define __UTIL_HASH_RIPEMD_HPP

#include <array>


///////////////////////////////////////////////////////////////////////////////
namespace util { namespace hash {
    class RIPEMD {
        public:
            typedef std::array<uint8_t,20> digest_t;

        public:
            RIPEMD();

            void update (const void *restrict, size_t) noexcept;
            void update (const uint8_t *restrict first, const uint8_t *restrict last) noexcept;

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
} }

#endif
