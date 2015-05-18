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

#ifndef __UTIL_NOISE_BASIS_HPP
#define __UTIL_NOISE_BASIS_HPP

#include <cstdlib>
#include "../lerp.hpp"
#include "../range.hpp"

namespace util {
    namespace noise {
        typedef double (*lerp_function)(double, double, double);

        struct basis {
            typedef uint64_t seed_t;

            basis (seed_t);
            basis ();
            virtual ~basis ();

            seed_t seed;

            virtual range<double> bounds (void) const = 0;
            virtual double eval (double x, double y) const = 0;
        };


        /// Perlin: single value per grid space
        template <lerp_function L>
        struct value : public basis {
            value (seed_t);
            value ();

            virtual range<double> bounds (void) const;
            virtual double eval (double x, double y) const;
        };


        /// Perlin: interpolated value across each grid space
        template <lerp_function L>
        struct gradient : public basis {
            gradient (seed_t);
            gradient ();

            virtual range<double> bounds (void) const;
            virtual double eval (double x, double y) const;
        };


        /// Cellular/Worley/Vornoi of order-1
        struct cellular : public basis {
            cellular (seed_t);
            cellular ();

            virtual range<double> bounds (void) const;
            virtual double eval (double x, double y) const;
        };
    }
}

#endif
