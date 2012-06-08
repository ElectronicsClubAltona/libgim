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


        template <lerp_function L> struct value : public basis {
            value (seed_t);
            value ();

            virtual range<double> bounds (void) const;
            virtual double eval (double x, double y) const;
        };

        template <lerp_function L> struct gradient : public basis {
            gradient (seed_t);
            gradient ();

            virtual range<double> bounds (void) const;
            virtual double eval (double x, double y) const;
        };

        struct cellular : public basis {
            cellular (seed_t);
            cellular ();

            virtual range<double> bounds (void) const;
            virtual double eval (double x, double y) const;
        };
    }
}

#endif
