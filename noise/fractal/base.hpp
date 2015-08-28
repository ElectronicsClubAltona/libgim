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

#ifndef __UTIL_NOISE_FRACTAL_BASE_HPP
#define __UTIL_NOISE_FRACTAL_BASE_HPP

#include <cstdint>

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
    struct base {
        using seed_t = uint64_t;

        // constructors
        base (seed_t,
              unsigned octaves,
              T H,
              T frequency,
              T lacunarity,
              T amplitude,
              T gain);

        // accessors
        constexpr unsigned octaves (void) const;
        unsigned octaves (unsigned);

        constexpr T H (void) const;
        T H (T);

        constexpr T frequency (void) const;
        T frequency (T);

        constexpr T lacunarity (void) const;
        T lacunarity (T);

        constexpr T amplitude (void) const;
        T amplitude (T);

        constexpr T gain (void) const;
        T gain (T);

        seed_t seed (void) const;
        seed_t seed (seed_t);

        const B& basis (void) const;
        B& basis (void);

    protected:
        unsigned m_octaves;
        T m_H;

        T m_frequency;
        T m_lacunarity;

        T m_amplitude;
        T m_gain;

        B m_basis;

        T m_invAH;
        T m_invGH;
    };
} } }

#include "base.ipp"

#endif

