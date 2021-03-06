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

#include "murmur3.hpp"

#include "common.hpp"
#include "../../bitwise.hpp"

#include <algorithm>

using util::hash::murmur3;


///////////////////////////////////////////////////////////////////////////////
static
uint32_t
read_u32 (const uint8_t *bytes)
{
    return bytes[0] <<  0 |
           bytes[1] <<  8 |
           bytes[2] << 16 |
           bytes[3] << 24;
}


///////////////////////////////////////////////////////////////////////////////
// Finalization mix - force all bits of a hash block to avalanche
template <size_t DigestBits, size_t ArchBits>
uint32_t
murmur3<DigestBits,ArchBits>::mix (uint32_t h)
{
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}


//-----------------------------------------------------------------------------
template <size_t DigestBits, size_t ArchBits>
uint64_t
murmur3<DigestBits,ArchBits>::mix (uint64_t k)
{
    k ^= k >> 33;
    k *= 0xff51afd7ed558ccd;
    k ^= k >> 33;
    k *= 0xc4ceb9fe1a85ec53;
    k ^= k >> 33;

    return k;
}


///////////////////////////////////////////////////////////////////////////////
template <size_t DigestBits, size_t ArchBits>
struct hash { };


//-----------------------------------------------------------------------------
template <size_t ArchBits>
struct hash<32,ArchBits> {
    static auto eval (util::view<const uint8_t*> data, uint32_t seed)
    {
        auto nblocks = data.size () / sizeof (uint32_t);

        uint32_t h1 = seed;

        static const uint32_t c1 = 0xcc9e2d51;
        static const uint32_t c2 = 0x1b873593;

        //----------
        // body
        auto cursor = data.begin ();
        auto last   = cursor + nblocks * sizeof (uint32_t);
        for (; cursor < last; cursor += sizeof (uint32_t)) {
            uint32_t k1 = read_u32 (cursor);

            k1 *= c1;
            k1 = util::rotatel (k1, 15);
            k1 *= c2;
            h1 ^= k1;

            h1 = util::rotatel (h1, 13);
            h1 += 0;
            h1 = h1 * 5 + 0xe6546b64;
        }

        //----------
        // tail
        if (data.size () % sizeof (uint32_t)) {
            uint32_t k1 = 0 ^ util::hash::murmur::tail<uint32_t> (cursor, data.size ());

            k1 *= c1;
            k1  = util::rotatel (k1, 15);
            k1 *= c2;
            h1 ^= k1;
        }

        //----------
        // finalization

        h1 ^= data.size ();
        h1  = util::hash::murmur3<32,ArchBits>::mix (h1);

        return h1;
    }
};


///////////////////////////////////////////////////////////////////////////////
template <typename T>
struct constants {
    T c;
    T Ks;
    T Hs;
    uint32_t O;
};


//-----------------------------------------------------------------------------
template <typename T>
struct traits {
    static constexpr size_t COMPONENTS = 16 / sizeof (T);
    static const constants<T> X[COMPONENTS];
};


//-----------------------------------------------------------------------------
template <>
const constants<uint32_t>
traits<uint32_t>::X[] = {
    { 0x239b961b, 15, 19, 0x561ccd1b },
    { 0xab0e9789, 16, 17, 0x0bcaa747 },
    { 0x38b34ae5, 17, 15, 0x96cd1c35 },
    { 0xa1e38b93, 18, 13, 0x32ac3b17 },
};


//-----------------------------------------------------------------------------
template <>
const constants<uint64_t>
traits<uint64_t>::X[] = {
    { 0x87c37b91114253d5, 31, 27, 0x52dce729 },
    { 0x4cf5ad432745937f, 33, 31, 0x38495ab5 }
};


///////////////////////////////////////////////////////////////////////////////
template <typename T>
T
half_round (std::array<T,traits<T>::COMPONENTS> h,
            std::array<T,traits<T>::COMPONENTS> k,
            size_t i)
{
    auto COMPONENTS = traits<T>::COMPONENTS;
    auto CONSTANTS  = traits<T>::X;

    auto i_ = (i + 1) % COMPONENTS;
    k[i] *= CONSTANTS[i].c;
    k[i]  = util::rotatel (k[i], CONSTANTS[i].Ks);
    k[i] *= CONSTANTS[i_].c;

    return h[i] ^= k[i];
}


//-----------------------------------------------------------------------------
template <typename T>
std::array<T,traits<T>::COMPONENTS>
full_round (std::array<T,traits<T>::COMPONENTS> h,
            std::array<T,traits<T>::COMPONENTS> k)
{
    auto COMPONENTS = traits<T>::COMPONENTS;
    auto CONSTANTS  = traits<T>::X;

    for (size_t i = 0; i < COMPONENTS; ++i) {
        h[i] = half_round (h, k, i);

        auto i_ = (i + 1) % COMPONENTS;
        h[i]  = util::rotatel (h[i], CONSTANTS[i].Hs);
        h[i] += h[i_];
        h[i]  = h[i] * 5 + CONSTANTS[i].O;
    }

    return h;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
std::array<T,traits<T>::COMPONENTS>
hash_128 (const void *restrict key,
          const size_t len,
          const uint32_t seed)
{
    // Initialise the hash
    static const size_t BLOCK_SIZE = 16;
    using result_t = std::array<T,traits<T>::COMPONENTS>;

    result_t h;
    h.fill (seed);

    // process the body
    auto cursor = reinterpret_cast<const T*> (key);
    auto last   = cursor + len / BLOCK_SIZE;
    for (; cursor < last; cursor += traits<T>::COMPONENTS) {
        result_t k;
        std::copy_n (cursor, traits<T>::COMPONENTS, k.begin ());

        h = full_round (h, k);
    }

    // process the tail
    if (len % 16) {
        auto k = util::hash::murmur::tail_array<T> (reinterpret_cast<const uint8_t*> (cursor), len);

        for (auto &v: k)
            v = 0 ^ v;

        for (size_t i = 0; i < traits<T>::COMPONENTS; ++i)
            h[i] = half_round (h, k, i);
    }

    // finalise the hash
    for (auto &v: h)
        v ^= len;

    for (size_t i = 1; i < traits<T>::COMPONENTS; ++i) h[0] += h[i];
    for (size_t i = 1; i < traits<T>::COMPONENTS; ++i) h[i] += h[0];

    for (auto &v: h)
        v = util::hash::murmur3<128,sizeof(T)*8>::mix (v);

    for (size_t i = 1; i < traits<T>::COMPONENTS; ++i) h[0] += h[i];
    for (size_t i = 1; i < traits<T>::COMPONENTS; ++i) h[i] += h[0];

    return h;
}


template <>
struct hash<128,32> {
    static auto eval (util::view<const uint8_t*> data, uint32_t seed)
    {
        return ::hash_128<uint32_t> (data.data (), data.size (), seed);
    }
};


template <>
struct hash<128,64> {
    static auto eval (util::view<const uint8_t*> data, uint32_t seed)
    {
        return ::hash_128<uint64_t> (data.data (), data.size (), seed);
    }
};


//-----------------------------------------------------------------------------
template <size_t DigestBits, size_t ArchBits>
typename murmur3<DigestBits,ArchBits>::digest_t
murmur3<DigestBits,ArchBits>::operator() (util::view<const uint8_t*> data) const noexcept
{
    return ::hash<DigestBits,ArchBits>::eval (data, m_seed);
}

///////////////////////////////////////////////////////////////////////////////
template class util::hash::murmur3<32, 32>;
template class util::hash::murmur3<128,32>;
template class util::hash::murmur3<128,64>;
