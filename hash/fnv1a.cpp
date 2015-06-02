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

template <size_t B>
struct constants { };

template <> struct constants<32> {
    static constexpr uint32_t PRIME =   16777619u;
    static constexpr uint32_t BIAS  = 2166136261u;
};

template <> struct constants<64> {
    static constexpr uint64_t PRIME = 1099511628211u;
    static constexpr uint64_t BIAS = 14695981039346656037u;
};


//-----------------------------------------------------------------------------
template <typename T>
T
fnv1a (const void *restrict _data, size_t len)
{
    auto *data = static_cast<const uint8_t *restrict> (_data);
    T result = constants<sizeof(T)*8>::BIAS;

    for (size_t i = 0; i < len; ++i) {
        result ^= data[i];
        result *= constants<sizeof(T)*8>::PRIME;
    }

    return result;
}


///////////////////////////////////////////////////////////////////////////////
uint32_t
util::hash::fnv1a32 (const void *data, size_t len)
{
    return fnv1a<uint32_t> (data, len);
}


//-----------------------------------------------------------------------------
uint64_t
util::hash::fnv1a64 (const void *data, size_t len)
{
    return fnv1a<uint64_t> (data, len);
}
