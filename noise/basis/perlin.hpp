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
 * Copyright 2012-2015 Danny Robson <danny@nerdcruft.net>
 */


#ifndef __UTIL_NOISE_BASIS_PERLIN_HPP
#define __UTIL_NOISE_BASIS_PERLIN_HPP

#include "../basis.hpp"
#include "../../point.hpp"
#include "../../range.hpp"

namespace util { namespace noise { namespace basis {
    /// Perlin: interpolated value across each grid space
    template <typename T, lerp_t<T> L>
    struct perlin {
        perlin (seed_t);
        perlin ();

        range<T> bounds (void) const;
        T operator() (point<2,T>) const;

        seed_t seed;

    private:
        constexpr vector<2,T> gradient (point<2,intmax_t>) const;
    };
} } }

#include "perlin.ipp"

#endif
