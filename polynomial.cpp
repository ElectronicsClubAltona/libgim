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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */


#include "polynomial.hpp"

#include "maths.hpp"

#include <limits>
#include <cmath>
#include <iostream>

static const size_t NEWTON_ITERATIONS = 1u;


//-----------------------------------------------------------------------------
namespace util { namespace polynomial {
    template <>
    std::array<float,1>
    solve (std::array<float,2> coeff)
    {
        const float a = coeff[0];
        const float b = coeff[1];

        if (almost_zero (a))
            return { std::numeric_limits<float>::quiet_NaN () };

        return { -b / a };
    }
} }

//-----------------------------------------------------------------------------
namespace util { namespace polynomial {
    template <>
    std::array<float,2>
    solve (std::array<float,3> coeff)
    {
        const float a = coeff[0];
        const float b = coeff[1];
        const float c = coeff[2];

        if (almost_zero (a)) {
            auto s = solve<1> ({b, c});
            return { s[0], std::numeric_limits<float>::quiet_NaN () };
        }

        auto d = std::sqrt (pow2 (b) - 4 * a * c);
        return { (-b - d) / (2 * a),
                 (-b + d) / (2 * a) };
    }
} }


//-----------------------------------------------------------------------------
// From graphics gems: http://tog.acm.org/resources/GraphicsGems/gemsiv/vec_mat/ray/solver.c
namespace util { namespace polynomial {
    template <>
    std::array<float,3>
    solve (std::array<float,4> coeffs)
    {
        const float _a = coeffs[0];
        const float _b = coeffs[1];
        const float _c = coeffs[2];
        const float _d = coeffs[3];

        // Take care of degenerate quadratic cases. We can also pass off if 'd'
        // is zero, but the benefit isn't clear given we have to merge results
        // at the end anyway.
        if (almost_zero (_a)) {
            auto s = solve<2> ({_b, _c, _d});
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
            s[1] = -t * std::cos (phi + PI_f / 3.f);
            s[2] = -t * std::cos (phi - PI_f / 3.f);
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
} }
