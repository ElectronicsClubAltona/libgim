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

#ifndef __UTIL_HASH_MURMUR_MURMUR2_HPP
#define __UTIL_HASH_MURMUR_MURMUR2_HPP

#include "../../view.hpp"

#include <cstddef>
#include <cstdint>

// Austin Appleby's MumurHash2, and MurmurHash64A. The exhaustive list of
// variants is deliberately not provided. You can damn well align your data or
// fix the algorithm.
namespace util::hash {
    namespace detail::murmur2 {
        template <typename T> struct constants;

        template <>
        struct constants<uint32_t> {
            static const uint32_t m = 0x5bd1e995;
            static const  int32_t r = 24;
        };

        template <>
        struct constants<uint64_t> {
            static const uint64_t m = 0xc6a4a7935bd1e995;
            static const  int64_t r = 47;
        };
    }

    template <typename DigestT>
    class murmur2 {
    public:
        using digest_t = DigestT;
        using seed_t = DigestT;

        murmur2 (seed_t _seed):
            m_seed (_seed)
        { ; }

        static constexpr uint32_t mix (uint32_t h, uint32_t k)
        {
            constexpr uint32_t m = detail::murmur2::constants<uint32_t>::m;
            constexpr uint32_t r = detail::murmur2::constants<uint32_t>::r;

            k *= m;
            k ^= k >> r;
            k *= m;

            h *= m;
            h ^= k;

            return h;
        }

        static constexpr uint64_t mix (uint64_t h, uint64_t k)
        {
            constexpr uint64_t m = detail::murmur2::constants<uint64_t>::m;
            constexpr uint64_t r = detail::murmur2::constants<uint64_t>::r;

            k *= m;
            k ^= k >> r;
            k *= m;

            h ^= k;
            h *= m;

            return h;
        }

        digest_t operator() (util::view<const uint8_t*> data) const noexcept;

    private:
        seed_t m_seed;
    };
}

#endif

