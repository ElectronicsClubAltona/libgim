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


struct params {
    typedef size_t seed_t;

    unsigned octaves;
    double   persistence;
    seed_t   seed;

    double 
    generate (intmax_t x, intmax_t y) {
        intmax_t mixed = rotate_left (x, sizeof (x) * 4) | y;
        seed_t   key   = (seed_t)mixed ^ seed;

        return util::hash (key) / (double)std::numeric_limits<intmax_t>::max ();
    }

    double
    smooth2d (intmax_t x, intmax_t y) {
        double corners = (generate(x - 1, y - 1) + 
                          generate(x + 1, y - 1) +
                          generate(x - 1, y + 1) +
                          generate(x + 1, y + 1)) / 16.0;
        double sides   = (generate(x - 1, y    ) +
                          generate(x + 1, y    ) +
                          generate(x,     y - 1) +
                          generate(x,     y + 1)) /  8.0;
        double center  =  generate(x,     y    )  /  4.0;
        
        return corners + sides + center;
    }

    

    params ():
        octaves     (16),
        persistence (0.7),
        seed        (util::random<size_t> ())
    { ; }
};


// Just a random generator [-1.0, 1.0]
static float
generate (intmax_t x, intmax_t y, size_t seed) {
    intmax_t n = x + 257 * y;
    n = (n << 13U) ^ n ^ seed;
    return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}


static double
smooth2d (intmax_t x, intmax_t y, size_t seed) {
    double corners = (generate(x - 1, y - 1, seed) + 
                      generate(x + 1, y - 1, seed) +
                      generate(x - 1, y + 1, seed) +
                      generate(x + 1, y + 1, seed)) / 16.0;
    double sides   = (generate(x - 1, y,     seed) +
                      generate(x + 1, y,     seed) +
                      generate(x,     y - 1, seed) +
                      generate(x,     y + 1, seed)) /  8.0;
    double center  =  generate(x,     y,     seed)  /  4.0;
    
    return corners + sides + center;
}


static double
curve (double t) {
    return t * t * (3.0 - 2.0 * t);
}


static double
sample2d (double x, double y, size_t seed) {
    intmax_t x_int = intmax_t (x);
    double   x_fac = x - x_int;
    intmax_t y_int = intmax_t (y);
    double   y_fac = y - y_int;

    double p0 = smooth2d (x_int,     y_int,     seed);
    double p1 = smooth2d (x_int + 1, y_int,     seed);
    double p2 = smooth2d (x_int,     y_int + 1, seed);
    double p3 = smooth2d (x_int + 1, y_int + 1, seed);

    double i0 = lerp::cosine (p0, p1, curve (x_fac));
    double i1 = lerp::cosine (p2, p3, curve (x_fac));

    double value = lerp::cosine (i0, i1, curve (y_fac));
    return value;
}


static double
sigmoid (double x) {
    return -1.0 + 2.0 / (1.0 + exp (-2.0 * x));
}


static double
perlin2d (double x, double y, size_t seed) {
    size_t levels      = 16;
    double persistence = 0.7;
    double total       = 0.0;

    for (size_t i = 0; i < levels; ++i) {
        double frequency = powf (2.0, i);
        double amplitude = powf (persistence, i);

        total += sample2d (x * frequency, y * frequency, seed) * amplitude;
    }

    return sigmoid (1.0 + total);
}


void
perlin2d (uint8_t *restrict pixels, size_t width, size_t height, const params& p) {
    for (size_t y = 0; y < height; ++y)
        for (size_t x = 0; x < width; ++x) {
            double p = perlin2d (x / 10.0, y / 10.0, (size_t)pixels);
            pixels[x + y * width] = p * std::numeric_limits<uint8_t>::max ();
        }
}


void
perlin2d (uint8_t *restrict pixels, size_t width, size_t height) {
    perlin2d (pixels, width, height, params ());
}

