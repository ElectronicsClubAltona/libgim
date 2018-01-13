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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_HASH_MURMUR_MURMUR1_HPP
#define __UTIL_HASH_MURMUR_MURMUR1_HPP

#include "../../view.hpp"

#include <cstdint>
#include <cstddef>

// Austin Appleby's MumurHash1
namespace util::hash {
    class murmur1 {
    public:
        using digest_t = uint32_t;
        using seed_t = uint32_t;

        murmur1 (seed_t _seed):
            m_seed (_seed)
        { ; }

        static constexpr uint32_t mix (uint32_t h, uint32_t k)
        {
            constexpr uint32_t m = 0xc6a4a793;

            h += k;
            h *= m;
            h ^= h >> 16;

            return h;
        }

        digest_t operator() (util::view<const uint8_t*> data) const noexcept;

    private:
        seed_t m_seed;
    };
}

#endif
