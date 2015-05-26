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
 * Copyright 2012 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_NOISE_FRACTAL_HPP
#define __UTIL_NOISE_FRACTAL_HPP

#include "basis.hpp"

namespace util {
    namespace noise {
        /// Base noise summation
        template <typename T>
        struct fractal {
            using seed_t = uint64_t;

            fractal (seed_t);
            fractal ();
            virtual ~fractal ();

            virtual T operator() (T x, T y) const = 0;

            seed_t seed;
        };


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
        struct fbm : public fractal<T> {
            using seed_t = typename basis<T>::seed_t;

            fbm (unsigned octaves,
                 T frequency,
                 T lacunarity,
                 T amplitude,
                 T gain,
                 seed_t seed);
            fbm ();

            unsigned octaves;

            T frequency;
            T lacunarity;

            T amplitude;
            T gain;

            B basis;
            virtual T operator() (T x, T y) const override;
        };


        /// Rigid Multifractal noise summation, based on Musgrave's algorithm
        ///
        /// octaves: count of layers to be summed
        /// frequency: point scaling factor for the base octave
        /// lacunarity: per octave frequency scaling factor
        template <typename T, typename B>
        struct rmf : public fractal<T> {
            using seed_t = typename basis<T>::seed_t;

            rmf (unsigned octaves,
                      T frequency,
                      T lacunarity,
                      T amplitude,
                      T gain,
                      seed_t seed);
            rmf ();

            unsigned octaves;

            T frequency;
            T lacunarity;

            T amplitude;
            T gain;

            B basis;
            virtual T operator() (T x, T y) const override;
        };
    }
}

#endif
