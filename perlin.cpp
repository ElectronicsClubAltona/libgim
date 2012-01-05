#include "perlin.hpp"

#include "lerp.hpp"
#include "debug.hpp"
#include "maths.hpp"

#include <cmath>
#include <limits>


// Just a random generator [-1.0, 1.0]
static float
generate (intmax_t x, intmax_t y) {
    uint32_t n = x + 257 * y;
    n = (n << 13U) ^ n;
    return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}


double
smooth2d (intmax_t x, intmax_t y) {
    double corners = (generate(x-1, y-1)+generate(x+1, y-1)+generate(x-1, y+1)+generate(x+1, y+1) ) / 16.0;
    double sides   = (generate(x-1, y)  +generate(x+1, y)  +generate(x, y-1)  +generate(x, y+1) ) /  8.0;
    double center  =  generate(x, y) / 4.0;
    
    return corners + sides + center;
}


double
curve (double t) {
    return t * t * (3.0 - 2.0 * t);
}


double
sample2d (double x, double y) {
    intmax_t x_int = intmax_t (x);
    double   x_fac = x - x_int;
    intmax_t y_int = intmax_t (y);
    double   y_fac = y - y_int;

    double p0 = smooth2d (x_int,     y_int);
    double p1 = smooth2d (x_int + 1, y_int);
    double p2 = smooth2d (x_int,     y_int + 1);
    double p3 = smooth2d (x_int + 1, y_int + 1);

    double i0 = lerp::cosine (p0, p1, curve (x_fac));
    double i1 = lerp::cosine (p2, p3, curve (x_fac));

    double value = lerp::cosine (i0, i1, curve (y_fac));
    return value;
}


double
sigmoid (double x) {
    return -1.0 + 2.0 / (1.0 + exp (-2.0 * x));
}


double
perlin2d (double x, double y) {
    size_t levels = 16;
    double persistence = 0.7;
    double total = 0.0;

    for (size_t i = 0; i < levels; ++i) {
        double frequency = powf (2.0, i);
        double amplitude = powf (persistence, i);

        total += sample2d (x * frequency, y * frequency) * amplitude;
    }

    return sigmoid (1.0 + total);
}


void
perlin2d (uint8_t *restrict pixels, size_t width, size_t height) {
    for (size_t y = 0; y < height; ++y)
        for (size_t x = 0; x < width; ++x) {
            double p = perlin2d (x / 10.0, y / 10.0);
            pixels[x + y * width] = p * std::numeric_limits<uint8_t>::max ();
        }
}

