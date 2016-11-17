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
 * Copyright 2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef CRUFT_UTIL_HASH_XXHASH_HPP
#define CRUFT_UTIL_HASH_XXHASH_HPP

#include <cstdint>
#include <type_traits>

namespace util::hash {
    template <typename T>
    class xxhash {
    public:
        static_assert (std::is_same<T,uint32_t>::value || std::is_same<T,uint64_t>::value);
        using digest_t = T;

        xxhash (void);
        xxhash (uint32_t seed);

        void update (const uint8_t *restrict first, const uint8_t *restrict last);
        void finish (void);
        
        digest_t digest (void) const;

        void reset (void);

    private:
        uint32_t m_seed;

        struct {
            uint32_t total_len_32;
            uint32_t large_len;

            T v1, v2, v3, v4;
            uint32_t mem32[4];
            uint32_t memsize;
            uint32_t reserved;

            //uint64_t length;
            //T v[4];
            //T mem[4];
            //unsigned memsize;
        } m_state;
    };

    using xxhash32 = xxhash<uint32_t>;
    using xxhash64 = xxhash<uint64_t>;
}

#endif
