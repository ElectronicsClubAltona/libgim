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

#ifndef __UTIL_RC4_HPP
#define __UTIL_RC4_HPP

#include <array>
#include <cstdint>
#include <cstddef>

namespace util { namespace crypto {
    class ARC4 {
    public:
        ARC4 (const uint8_t *restrict key, size_t len);

        void discard (size_t);
        void generate (uint8_t *restrict dst, size_t len);

    private:
        uint8_t get (void);

        size_t x, y;
        std::array<uint8_t,256> S;
    };
} }

#endif
