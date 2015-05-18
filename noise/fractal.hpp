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
        struct fractal {
            fractal (unsigned      octaves,
                     double        frequency,
                     double        lacunarity);
            fractal ();
            virtual ~fractal ();

            unsigned octaves;
            double   frequency;
            double   lacunarity;

            virtual double operator() (double x, double y) const { return eval (x, y); };
            virtual double eval (double x, double y) const = 0;
        };


        template <typename B>
        struct fbm : public fractal {
            fbm (unsigned octaves,
                 double frequency,
                 double lacunarity,
                 basis::seed_t seed);
            fbm ();

            B basis;
            virtual double eval (double x, double y) const;
        };

        template <typename B>
        struct musgrave : public fractal {
            musgrave (unsigned octaves,
                      double frequency,
                      double lacunarity,
                      basis::seed_t seed);
            musgrave ();

            B basis;
            virtual double eval (double x, double y) const;
        };
    }
}

#endif
