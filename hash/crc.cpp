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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#include "./crc.hpp"

#include "../debug.hpp"

#include <cruft/util/bitwise.hpp>

#include <array>

using util::hash::crc;


///////////////////////////////////////////////////////////////////////////////
template <
    typename DigestT,
    DigestT Generator,
    DigestT Initial,
    DigestT Final,
    bool ReflectIn,
    bool ReflectOut
>
const std::array<DigestT,256>
crc<
    DigestT,Generator,Initial,Final,ReflectIn,ReflectOut
>::s_table = crc<DigestT,Generator,Initial,Final,ReflectIn,ReflectOut>::table ();


///////////////////////////////////////////////////////////////////////////////
template <
    typename DigestT,
    DigestT Generator,
    DigestT Initial,
    DigestT Final,
    bool ReflectIn,
    bool ReflectOut
>
crc<
    DigestT,Generator,Initial,Final,ReflectIn,ReflectOut
>::crc () noexcept
{
    reset ();
}


//-----------------------------------------------------------------------------
template <
    typename DigestT,
    DigestT Generator,
    DigestT Initial,
    DigestT Final,
    bool ReflectIn,
    bool ReflectOut
>
void
crc<
    DigestT,Generator,Initial,Final,ReflectIn,ReflectOut
>::reset (void) noexcept
{
    m_digest = Initial;
}


///////////////////////////////////////////////////////////////////////////////
template <
    typename DigestT,
    DigestT Generator,
    DigestT Initial,
    DigestT Final,
    bool ReflectIn,
    bool ReflectOut
>
void
crc<
    DigestT,Generator,Initial,Final,ReflectIn,ReflectOut
>::update (const uint8_t *restrict first,
           const uint8_t *restrict last) noexcept
{
    CHECK_LE (first, last);

    for (auto cursor = first; cursor != last; ++cursor) {
        if (ReflectIn)
            m_digest = s_table[*cursor ^ (m_digest & 0xFFu)] ^ (m_digest >> 8u);
        else {
            constexpr auto shift = sizeof (DigestT) * 8u - 8u;
            m_digest = (m_digest << 8u) ^ s_table[(m_digest >> shift) ^ *cursor];
        }
    }
}


//-----------------------------------------------------------------------------
template <
    typename DigestT,
    DigestT Generator,
    DigestT Initial,
    DigestT Final,
    bool ReflectIn,
    bool ReflectOut
>
void
crc<
    DigestT,Generator,Initial,Final,ReflectIn,ReflectOut
>::update (const void *restrict _data, size_t len) noexcept
{
    auto data = reinterpret_cast<const uint8_t *restrict> (_data);
    return update(data, data + len);
}


//-----------------------------------------------------------------------------
template <
    typename DigestT,
    DigestT Generator,
    DigestT Initial,
    DigestT Final,
    bool ReflectIn,
    bool ReflectOut
>
void
crc<
    DigestT,Generator,Initial,Final,ReflectIn,ReflectOut
>::finish (void)
{
    ;
}


//-----------------------------------------------------------------------------
template <
    typename DigestT,
    DigestT Generator,
    DigestT Initial,
    DigestT Final,
    bool ReflectIn,
    bool ReflectOut
>
DigestT
crc<
    DigestT,Generator,Initial,Final,ReflectIn,ReflectOut
>::digest (void) const
{
    return (ReflectIn != ReflectOut ? util::reverse (m_digest) : m_digest) ^ Final;
}


///////////////////////////////////////////////////////////////////////////////
template <
    typename DigestT,
    DigestT Generator,
    DigestT Initial,
    DigestT Final,
    bool ReflectIn,
    bool ReflectOut
>
constexpr
std::array<DigestT,256>
util::hash::crc<
    DigestT,Generator,Initial,Final,ReflectIn,ReflectOut
>::table (void)
{
    // We want to use a std::array here for a (slight) safety advantage, but
    // it means that we have to use calls to array::data instead of operator[]
    // to retain constexpr.
    std::array<digest_t,256> values {};

    if (ReflectIn) {
        constexpr auto gen = util::reverse (Generator);

        for (int i = 0; i < 256; ++i) {
            digest_t c = i;

            for (int k = 0; k < 8; ++k) {
                if (c & 1)
                    c = gen ^ (c >> 1);
                else
                    c >>= 1;
            }

            values.data ()[i] = c;
        }
    } else {
        constexpr DigestT   shift =                sizeof (DigestT) * 8u - 8u;
        constexpr DigestT highbit = DigestT(1) << (sizeof (DigestT) * 8u - 1u);

        for (int idx = 0; idx < 256; ++idx) {
            DigestT c = DigestT (idx) << shift;

            for (int i = 0; i < 8; ++i) {
                if (c & highbit)
                    c = (c << DigestT (1)) ^ Generator;
                else
                    c <<= DigestT(1);
            }

            values.data()[idx] = c;
        }
    }

    return values;
};


///////////////////////////////////////////////////////////////////////////////
template class util::hash::crc<uint32_t, 0x04C11DB7, 0xffffffff, 0xffffffff, true,  true >; // crc32
template class util::hash::crc<uint32_t, 0x04C11DB7, 0xffffffff, 0xffffffff, false, false>; // crc32b
template class util::hash::crc<uint32_t, 0x1edc6f41, 0xffffffff, 0xffffffff, true,  true>;  // crc32c
template class util::hash::crc<uint32_t, 0xa833982b, 0xffffffff, 0xffffffff, true,  true>;  // crc32d

template class util::hash::crc<uint32_t, 0x04C11DB7, 0x00000000, 0x00000000, false, false>; // ogg

template class util::hash::crc<uint64_t, 0x42f0e1eba9ea3693, 0, 0, false, false>; // crc64