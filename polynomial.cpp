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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */


#include "polynomial.hpp"

#include "maths.hpp"

#include <limits>
#include <cmath>

static const size_t NEWTON_ITERATIONS = 1u;


//-----------------------------------------------------------------------------
namespace util::polynomial {
    template <>
    std::array<float,1>
    roots (std::array<float,2> coeff)
    {
        const float a = coeff[0];
        const float b = coeff[1];

        if (almost_zero (a))
            return { std::numeric_limits<float>::quiet_NaN () };

        return { -b / a };
    }
}


//-----------------------------------------------------------------------------
namespace util::polynomial {
    template <>
    std::array<float,2>
    roots (std::array<float,3> coeff)
    {
        const float a = coeff[0];
        const float b = coeff[1];
        const float c = coeff[2];

        if (almost_zero (a)) {
            auto s = roots<1> ({b, c});
            return { s[0], std::numeric_limits<float>::quiet_NaN () };
        }

        auto descriminator = std::sqrt (pow2 (b) - 4 * a * c);
        return {
            (-b - descriminator) / (2 * a),
            (-b + descriminator) / (2 * a)
        };
    }
}


//-----------------------------------------------------------------------------
// From graphics gems: http://tog.acm.org/resources/GraphicsGems/gemsiv/vec_mat/ray/solver.c
namespace util::polynomial {
    template <>
    std::array<float,3>
    roots (std::array<float,4> coeffs)
    {
        const float _a = coeffs[0];
        const float _b = coeffs[1];
        const float _c = coeffs[2];
        const float _d = coeffs[3];

        // Take care of degenerate quadratic cases. We can also pass off if 'd'
        // is zero, but the benefit isn't clear given we have to merge results
        // at the end anyway.
        if (almost_zero (_a)) {
            auto s = roots<2> ({_b, _c, _d});
            return {s[0], s[1], std::numeric_limits<float>::quiet_NaN () };
        }

        std::array<float,3> s;

        // Normalise to x^3 + ax^2 + bx + c = 0
        const float a = _b / _a;
        const float b = _c / _a;
        const float c = _d / _a;

        // Substituate x = y - a / 3 to eliminate the quadric. Now: x^3 + px + q = 0
        const float p = (-a * a / 3.f + b) / 3.f;
        const float q = (2 * a * a * a / 27.f - a * b /3.f + c) / 2.f;

        // Polynomial descriminant
        const float D = q * q + p * p * p;

        // Solve using Cardano's method
        if (almost_zero (D))
        {
            if (almost_zero (q)) {
                s[0] = 0.f;
                s[1] = std::numeric_limits<float>::quiet_NaN ();
                s[2] = std::numeric_limits<float>::quiet_NaN ();
            } else {
                const float u = std::cbrt (-q);
                s[0] = 2 * u;
                s[1] = -u;
                s[2] = std::numeric_limits<float>::quiet_NaN ();
            }
        } else if (D < 0) {
            const float phi = std::acos (-q / std::sqrt (-p * p * p)) / 3.f;
            const float t   = 2 * std::sqrt (-p);

            s[0] =  t * std::cos (phi);
            s[1] = -t * std::cos (phi + PI<float> / 3.f);
            s[2] = -t * std::cos (phi - PI<float> / 3.f);
        } else {
            float u = std::cbrt (std::sqrt (D) + abs (q));
            if (q > 0.f)
                s[0] = -u + p / u;
            else
                s[0] =  u - p / u;

            s[1] = std::numeric_limits<float>::quiet_NaN ();
            s[2] = std::numeric_limits<float>::quiet_NaN ();
        }

        // Resubstitute a / 3 from above
        const float sub = a / 3.f;
        for (auto &i: s)
            i -= sub;

        // Run some iterations of Newtons method to make the results slightly
        // more accurate, they're a little loose straight out of the bat.
        const float da = 3 * _a;
        const float db = 2 * _b;
        const float dc = 1 * _c;

        for (auto &i: s) {
            for (size_t j = 0; j < NEWTON_ITERATIONS; ++j) {
                float deriv = da * i * i + db * i + dc;
                if (almost_zero (deriv))
                    continue;

                i = i - eval (coeffs, i) / deriv;
            }
        }

        return s;
    }
}
