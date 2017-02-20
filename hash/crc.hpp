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

#ifndef __UTIL_HASH_CRC_HPP
#define __UTIL_HASH_CRC_HPP

#include <array>
#include <cstdint>
#include <cstdlib>
#include <type_traits>


///////////////////////////////////////////////////////////////////////////////
namespace util { namespace hash {
    // Implements the crc checksum (from ethernet, png, etc).
    //
    // Adapted from the PNG specification (ISO/IEC 15948:2003), appendix D and
    // the public domain implementation of Ross Williams.
    //
    // Generator: the polynomial, with the leading (ie, 32nd) high bit truncated.
    // Initial:   value used to initialise the digest
    // Final: value to xor against the digest at finish time
    // ReflectIn: whether to reverse the bits of each data byte
    // ReflectOut: whether to reverse the bits of the digest at finish time
    //
    // Note that reflection isn't necessarily explicitly performed at update
    // time. Instead we construct the lookup table appropriately to directly
    // use the data values directly.
    template <
        typename DigestT,
        DigestT Generator,
        DigestT Initial,
        DigestT Final,
        bool ReflectIn,
        bool ReflectOut
    >
    class crc {
    public:
        using digest_t = DigestT;

        static constexpr auto generator = Generator;

        crc () noexcept;

        void reset (void) noexcept;

        void update (const void *restrict data, size_t bytes) noexcept;
        void update (const uint8_t *restrict first, const uint8_t *restrict last) noexcept;

        void finish (void);

        digest_t digest (void) const;

        static constexpr
        std::array<DigestT,256>
        table (void);

    private:
        digest_t m_digest;

        static const std::array<DigestT,256> s_table;
    };

    using crc32  = crc<uint32_t, 0x04c11db7, 0xffffffff, 0xffffffff, true,  true>;
    using crc32b = crc<uint32_t, 0x04c11db7, 0xffffffff, 0xffffffff, false, false>;
    using crc32c = crc<uint32_t, 0x1edc6f41, 0xffffffff, 0xffffffff, true,  true>;
    using crc32d = crc<uint32_t, 0xa833982b, 0xffffffff, 0xffffffff, true,  true>;

    using crc64 = crc<uint64_t, 0x42f0e1eba9ea3693, 0, 0, false, false>;
} }

#endif
