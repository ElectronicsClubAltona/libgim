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

#ifndef __UTIL_NOISE_FRACTAL_FBM_HPP
#define __UTIL_NOISE_FRACTAL_FBM_HPP

#include <cstdint>

#include "base.hpp"
#include "../../point.hpp"

namespace util { namespace noise { namespace fractal {
    /// Fractal Brownian Motion summation.
    ///
    /// Sum progressive layers of a noise basis with scaling frequency
    /// and amplitude.
    ///
    /// octaves: count of layers to be summed
    /// frequency: point scaling factor for the base octave
    /// lacunarity: per octave frequency scaling factor
    /// amplitude: maximum absolute value of the noise
    /// gain: per octave amplitude scaling factor. typically 1/f.
    template <typename T, typename B>
    struct fbm : public base<T,B> {
        using seed_t = typename base<T,B>::seed_t;

        static constexpr unsigned DEFAULT_OCTAVES = 8;
        static constexpr T DEFAULT_H          = 1;
        static constexpr T DEFAULT_FREQUENCY  = T(0.1);
        static constexpr T DEFAULT_LACUNARITY = 2;
        static constexpr T DEFAULT_AMPLITUDE  = 1;
        static constexpr T DEFAULT_GAIN       = 1 / DEFAULT_LACUNARITY;

        fbm (seed_t seed,
             unsigned octaves,
             T H,
             T frequency,
             T lacunarity,
             T amplitude,
             T gain);
        fbm (seed_t);

        T operator() (util::point<2,T>) const;
    };
} } }

#include "fbm.ipp"

#endif
