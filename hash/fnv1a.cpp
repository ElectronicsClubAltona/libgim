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
 * Copyright 2010-2015 Danny Robson <danny@nerdcruft.net>
 */

#include "fnv1a.hpp"

using util::hash::fnv1a;


///////////////////////////////////////////////////////////////////////////////
// Prime is,
//   32: 2^ 24 + 2^8 + 0x93
//   64: 2^ 40 + 2^8 + 0xb3
//  128: 2^ 88 + 2^8 + 0x3B
//  256: 2^168 + 2^8 + 0x63
//  512: 2^344 + 2^8 + 0x57
// 1024: 2^680 + 2^8 + 0x8D
//
// Bias is the FNV-0 hash of "chongo <Landon Curt Noll> /\\../\\"

template <typename DigestT>
struct constants { };


//-----------------------------------------------------------------------------
template <>
struct constants<uint32_t> {
    static constexpr uint32_t prime =   16777619u;
    static constexpr uint32_t bias  = 2166136261u;
};


//-----------------------------------------------------------------------------
template <>
struct constants<uint64_t> {
    static constexpr uint64_t prime = 1099511628211u;
    static constexpr uint64_t bias  = 14695981039346656037u;
};


///////////////////////////////////////////////////////////////////////////////
template <typename DigestT>
typename fnv1a<DigestT>::digest_t
fnv1a<DigestT>::operator() (const util::view<const uint8_t*> data) const noexcept
{
    auto result = constants<DigestT>::bias;

    for (auto i: data) {
        result ^= i;
        result *= constants<DigestT>::prime;
    }

    return result;
}


template struct util::hash::fnv1a<uint32_t>;
template struct util::hash::fnv1a<uint64_t>;
