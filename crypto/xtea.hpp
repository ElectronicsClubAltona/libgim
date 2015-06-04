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

#ifndef __UTIL_TEA_HPP
#define __UTIL_TEA_HPP

#include <array>
#include <cstdint>
#include <cstddef>

namespace util { namespace crypto {
    // http://en.wikipedia.org/wiki/XTEA
    class XTEA {
    public:
        using key_t = std::array<uint32_t,4>;

        explicit XTEA (key_t);

        void encrypt (uint32_t *restrict data, size_t count);
        void decrypt (uint32_t *restrict data, size_t count);

    private:
        key_t m_key;
    };
} }

#endif
