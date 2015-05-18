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
            fractal (unsigned octaves,
                     T frequency,
                     T lacunarity);
            fractal ();
            virtual ~fractal ();

            unsigned octaves;
            T frequency;
            T lacunarity;

            virtual T operator() (T x, T y) const = 0;
        };


        /// Fractal Brownian Motion summation.
        template <typename T, typename B>
        struct fbm : public fractal<T> {
            using seed_t = typename basis<T>::seed_t;

            fbm (unsigned octaves,
                 T frequency,
                 T lacunarity,
                 seed_t seed);
            fbm ();

            B basis;
            virtual T operator() (T x, T y) const override;
        };


        /// Rigid Multifractal noise summation.
        template <typename T, typename B>
        struct musgrave : public fractal<T> {
            using seed_t = typename basis<T>::seed_t;

            musgrave (unsigned octaves,
                      T frequency,
                      T lacunarity,
                      seed_t seed);
            musgrave ();

            B basis;
            virtual T operator() (T x, T y) const override;
        };
    }
}

#endif
