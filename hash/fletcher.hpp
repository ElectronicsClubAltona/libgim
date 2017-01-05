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
 * Copyright 2010-2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_HASH_FLETCHER_HPP
#define __UTIL_HASH_FLETCHER_HPP

#include "../types/bits.hpp"

#include <cstdint>
#include <cstdlib>


///////////////////////////////////////////////////////////////////////////////
namespace util::hash {
    template <typename DIGEST>
    class fletcher {
    public:
        using digest_t = DIGEST;
        using part_t = typename bytes_type<sizeof (digest_t) / 2>::uint;

        fletcher (part_t modulus, part_t a, part_t b);

        void update (const void *restrict, size_t) noexcept;
        void update (const uint8_t *restrict first, const uint8_t *restrict last) noexcept;

        void finish (void);
        digest_t digest (void) const;

        void reset (void);

    private:
        const digest_t m_modulus;

        struct state_t {
            part_t a, b;
        };

        const state_t m_initial;
        state_t m_state;
    };
}


#endif

