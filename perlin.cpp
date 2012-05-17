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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */


#include "perlin.hpp"

#include "bitwise.hpp"
#include "debug.hpp"
#include "hash.hpp"
#include "lerp.hpp"
#include "maths.hpp"
#include "random.hpp"

#include <cmath>
#include <limits>


#define LERP(a,b,t) lerp::quintic (a, b, t)


struct params {
    typedef size_t seed_t;

    unsigned octaves;
    double   persistence;
    double   frequency;
    seed_t   seed;

    // Just a random generator [-1.0, 1.0]
    double 
    generate (intmax_t x, intmax_t y) const {
        intmax_t n = x + 257 * y;
        n = (n << 13U) ^ n ^ (intmax_t)seed;
        return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
    }

    double
    eval (double x, double y) const {
        intmax_t x_int = intmax_t (x);
        double   x_fac = x - x_int;
        intmax_t y_int = intmax_t (y);
        double   y_fac = y - y_int;

        // Generate the four corner values
        double p0 = generate (x_int,     y_int);
        double p1 = generate (x_int + 1, y_int);
        double p2 = generate (x_int,     y_int + 1);
        double p3 = generate (x_int + 1, y_int + 1);

        // Interpolate on one dimension, then the other.
        return LERP (LERP (p0, p1, x_fac),
                     LERP (p2, p3, x_fac),
                     y_fac);
    }

    double
    sample (double x, double y) const {
        double total       = 0.0;

        for (size_t i = 0; i < octaves; ++i) {
            double f = frequency * powf (2.0, i);
            double amplitude = powf (persistence, i);

            total += eval (x * f, y * f) * amplitude;
            total = max (-1.0, min (1.0, total));
        }

        return (1.0 + total) / 2.0;
    }

    params ():
        octaves     (3),
        persistence (0.5),
        frequency   (25.0),
        seed        (util::random<size_t> ())
    { ; }
};


static void
perlin2d (uint8_t *restrict pixels, size_t width, size_t height, const params& p) {
    for (size_t y = 0; y < height; ++y)
        for (size_t x = 0; x < width; ++x) {
            double v = p.sample (x / double (width), y / double (height)); /*perlin2d (x / double (width),
                                        y / double (height),
                                 (size_t)pixels);*/
            pixels[x + y * width] = v * std::numeric_limits<uint8_t>::max ();
        }
}


void
perlin2d (uint8_t *restrict pixels, size_t width, size_t height) {
    perlin2d (pixels, width, height, params ());
}
