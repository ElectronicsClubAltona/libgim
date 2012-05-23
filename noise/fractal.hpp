/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
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
