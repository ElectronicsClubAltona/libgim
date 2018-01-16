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

#ifndef __UTIL_HASH_MURMUR_MURMUR3_HPP
#define __UTIL_HASH_MURMUR_MURMUR3_HPP

#include "../../view.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

// Austin Appleby's MurmurHash3
namespace util::hash {
    namespace detail::murmur3 {
        template <size_t DigestBits, size_t ArchBits>
        struct digest_type { };

        template <> struct digest_type< 32,32> { using type = uint32_t; };
        template <> struct digest_type<128,32> { using type = std::array<uint32_t,4>; };
        template <> struct digest_type<128,64> { using type = std::array<uint64_t,2>; };
    };

    template <size_t DigestBits, size_t ArchBits>
    class murmur3 {
    public:
        murmur3 (uint32_t _seed):
            m_seed (_seed)
        { ; }

        static_assert (DigestBits % 8 == 0);

        using digest_t = typename detail::murmur3::digest_type<DigestBits,ArchBits>::type;

        static uint32_t mix (uint32_t);
        static uint64_t mix (uint64_t);

        digest_t
        operator() (util::view<const uint8_t*> data) const noexcept;

    private:
        uint32_t m_seed;
    };

    using murmur3_32      = murmur3< 32,32>;
    using murmur3_128_x86 = murmur3<128,32>;
    using murmur3_128_x64 = murmur3<128,64>;
}

#endif

