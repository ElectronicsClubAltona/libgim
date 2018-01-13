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

#include "../view.hpp"

#include <cstdint>
#include <type_traits>

namespace util::hash {
    template <typename T>
    class xxhash {
    public:
        static_assert (std::is_same<T,uint32_t>::value || std::is_same<T,uint64_t>::value);
        using digest_t = T;

        static constexpr uint32_t DEFAULT_SEED = 0;

        xxhash (uint32_t seed = DEFAULT_SEED);

        digest_t operator() (const util::view<const uint8_t*> data);

    private:
        uint32_t m_seed;
    };

    using xxhash32 = xxhash<uint32_t>;
    using xxhash64 = xxhash<uint64_t>;
}

#endif
