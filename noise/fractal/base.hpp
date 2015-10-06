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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_NOISE_FRACTAL_BASE_HPP
#define __UTIL_NOISE_FRACTAL_BASE_HPP

#include <cstdint>

#include "../../point.hpp"
#include "../rand.hpp"

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
    struct base {
        using value_t = typename B::value_t;
        using point_t = typename B::point_t;

        // constructors
        base (seed_t,
              unsigned octaves,
              value_t H,
              value_t frequency,
              value_t lacunarity,
              value_t amplitude,
              value_t gain);

        // accessors
        constexpr unsigned octaves (void) const;
        unsigned octaves (unsigned);

        constexpr value_t H (void) const;
        value_t H (value_t);

        constexpr value_t frequency (void) const;
        value_t frequency (value_t);

        constexpr value_t lacunarity (void) const;
        value_t lacunarity (value_t);

        constexpr value_t amplitude (void) const;
        value_t amplitude (value_t);

        constexpr value_t gain (void) const;
        value_t gain (value_t);

        seed_t seed (void) const;
        seed_t seed (seed_t);

        const B& basis (void) const;
        B& basis (void);

    protected:
        unsigned m_octaves;
        value_t m_H;

        value_t m_frequency;
        value_t m_lacunarity;

        value_t m_amplitude;
        value_t m_gain;

        B m_basis;

        value_t m_invAH;
        value_t m_invGH;
    };
} } }

#include "base.ipp"

#endif

