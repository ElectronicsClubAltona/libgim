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
 * Copyright 2013 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_HASH_MD2_HPP
#define __UTIL_HASH_MD2_HPP

#include <array>
#include <cstdint>


namespace util {
    namespace hash {
        class MD2 {
            public:
                typedef std::array<uint8_t,16> digest_t;

            public:
                MD2 ();

                void update (const uint8_t *data, size_t len);
                void update (const void    *data, size_t len);

                void finish (void);
                digest_t digest (void) const;
                void reset (void);

            private:
                void transform (void);

                uint64_t m_total;

                uint8_t C[16];
                uint8_t X[48];
        };
    }
}

#endif
