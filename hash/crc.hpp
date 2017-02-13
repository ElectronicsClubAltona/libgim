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
namespace util::hash {
    // Implements the crc checksum (from ethernet, png, etc).
    // Adapted from the PNG specification (ISO/IEC 15948:2003), appendix D.
    template <typename DigestT = uint32_t, DigestT Generator = 0xEDB88320>
    class crc {
    public:
        static_assert (std::is_same<DigestT, std::uint32_t>::value,
                       "only 32 bit crc is supported at this time");

        using digest_t = DigestT;

        crc () noexcept;

        void reset (void) noexcept;

        void update (const void *restrict data, size_t bytes) noexcept;
        void update (const uint8_t *restrict first, const uint8_t *restrict last) noexcept;

        void finish (void);

        digest_t digest (void) const;

        static
        std::array<DigestT,256>
        table (void);

    private:
        digest_t m_digest;

        static const std::array<DigestT,256> s_table;
    };

    using crc32 = crc<uint32_t, 0xEDB88320>;
}

#endif
