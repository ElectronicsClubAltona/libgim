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

#ifndef __UTIL_NOISE_BASIS_PATCH_HPP
#define __UTIL_NOISE_BASIS_PATCH_HPP

#include "./type/distance.hpp"

#include "../fwd.hpp"
#include "../../point.hpp"

namespace util { namespace noise { namespace basis {
    template <size_t S, typename T>
    struct patch : public type::distance<S,2> {
        patch (seed_t, T width = 0);
        
        range<T> bounds (void) const;

        T operator() (point<S,T>) const;
        
        seed_t seed (void) const;
        seed_t seed (seed_t);

        T width (void) const;
        T width (T);

    private:
        static constexpr T THRESHOLD = 1 - T(0.999);

        T m_width;
        T m_power;
        seed_t m_seed;
    };
} } }

#include "patch.ipp"

#endif
