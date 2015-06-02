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

#ifndef __UTIL_NOISE_FRACTAL_RMF_HPP
#define __UTIL_NOISE_FRACTAL_RMF_HPP

#include <cstdint>

#include "base.hpp"
#include "../../point.hpp"

namespace util { namespace noise { namespace fractal {
    /// Rigid Multifractal summation, based on Musgrave's algorithm
    ///
    /// octaves: count of layers to be summed
    /// H: hurst parameter (~roughness)
    /// offset: TODO
    /// frequency: point scaling factor for the base octave
    /// lacunarity: incremental octave frequency scaling factor
    /// amplitude: value scaling factor for the base octave
    /// gain: incremental octave value scaling factor
    template <typename T, typename B>
    struct rmf : public base<T,B> {
        using seed_t = typename base<T,B>::seed_t;

        static constexpr unsigned DEFAULT_OCTAVES = 5;
        static constexpr T DEFAULT_H = 1;
        static constexpr T DEFAULT_OFFSET = 1;
        static constexpr T DEFAULT_FREQUENCY = 1;
        static constexpr T DEFAULT_LACUNARITY = 2;
        static constexpr T DEFAULT_AMPLITUDE = 2;
        static constexpr T DEFAULT_GAIN = 1 / DEFAULT_LACUNARITY;

        rmf (seed_t,
             unsigned octaves,
             T H,
             T frequency,
             T lacunarity,
             T amplitude,
             T gain,
             T offset);

        rmf (seed_t);

        constexpr T operator() (util::point<2,T>) const;

    private:
        T m_offset;
    };
} } }

#include "rmf.ipp"

#endif
