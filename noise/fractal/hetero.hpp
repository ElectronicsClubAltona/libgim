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

#ifndef __UTIL_NOISE_FRACTAL_HETERO_HPP
#define __UTIL_NOISE_FRACTAL_HETERO_HPP

#include <cstdint>

#include "../../point.hpp"

namespace util { namespace noise { namespace fractal {
    ///////////////////////////////////////////////////////////////////////
    /// Heterogeneous procedural terrain fucntion: stats by altitude method
    template <typename T, typename B>
    struct hetero {
        using seed_t = uint64_t;

        hetero ();

        seed_t seed;
        T H;
        unsigned octaves;

        T frequency;
        T lacunarity;

        T offset;

        T amplitude;
        T gain;

        B basis;

        constexpr T operator() (util::point<2,T>) const;

    private:
        T invAH;
        T invGH;
    };
} } }

#include "hetero.ipp"

#endif


