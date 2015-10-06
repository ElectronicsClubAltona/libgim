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

#ifndef __UTIL_NOISE_FRACTAL_HMF_HPP
#define __UTIL_NOISE_FRACTAL_HMF_HPP

#include <cstdint>

#include "../../point.hpp"

namespace util { namespace noise { namespace fractal {
    ///////////////////////////////////////////////////////////////////////
    /// Musgrave's "Hybrid MultiFractal"
    template <
        size_t S,   // probe point dimensionality
        typename T, // probe point value_type
        typename B  // generating basis function
    >
    struct hmf : public base<S,T,B> {
        using seed_t = typename base<S,T,B>::seed_t;

        // H should be fairly low due to the decreasing weight parameter in eval
        static constexpr unsigned DEFAULT_OCTAVES = 6;
        static constexpr T DEFAULT_H = T(0.25);
        static constexpr T DEFAULT_FREQUENCY = T(0.1);
        static constexpr T DEFAULT_LACUNARITY = 2;
        static constexpr T DEFAULT_AMPLITUDE = 1;
        static constexpr T DEFAULT_GAIN = 1 / DEFAULT_LACUNARITY;
        static constexpr T DEFAULT_OFFSET = T(0.7);

        hmf (seed_t,
             unsigned octaves,
             T H,
             T frequency,
             T lacunarity,
             T amplitude,
             T gain,
             T offset);

        hmf (seed_t);

        T operator() (point<S,T>) const;

    private:
        T m_offset;
    };
} } }

#include "hmf.ipp"

#endif

