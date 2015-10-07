/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUvalue_t WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
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
    template <class B>
    struct fbm : public base<B> {
        using value_t = typename base<B>::value_t;
        using point_t = typename base<B>::point_t;

        static constexpr unsigned DEFAULT_OCTAVES = 8;
        static constexpr value_t DEFAULT_H          = 1;
        static constexpr value_t DEFAULT_FREQUENCY  = value_t(0.1);
        static constexpr value_t DEFAULT_LACUNARITY = 2;
        static constexpr value_t DEFAULT_AMPLITUDE  = 1;
        static constexpr value_t DEFAULT_GAIN       = 1 / DEFAULT_LACUNARITY;

        fbm (seed_t seed,
             unsigned octaves,
             value_t H,
             value_t frequency,
             value_t lacunarity,
             value_t amplitude,
             value_t gain);
        fbm (seed_t);

        value_t operator() (point_t) const noexcept;
    };
} } }

#include "fbm.ipp"

#endif
