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
            auto s = solve<2> ({b, c});
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
    solve (std::array<float,4> coeff)
    {
        const float _a = coeff[0];
        const float _b = coeff[1];
        const float _c = coeff[2];
        const float _d = coeff[3];

        if (almost_zero (_a)) {
            auto s = solve<3> ({_b, _c, _d});
            return {s[0], s[1], std::numeric_limits<float>::quiet_NaN () };
        }

        std::array<float,3> s = {
            std::numeric_limits<float>::quiet_NaN (),
            std::numeric_limits<float>::quiet_NaN (),
            std::numeric_limits<float>::quiet_NaN ()
        };

        // Normalise to x^3 + ax^2 + bx + c = 0
        const float a = _b / _a;
        const float b = _c / _a;
        const float c = _d / _a;

        // Substituate x = y - a / 3 to eliminate the quadric. Now: x^3 + px + q = 0
        float p = (-1 / 3.f * a * a + b) / 3.f;
        float q = (2 / 27.f * a * a * a - 1 / 3.f * a * b + c) / 2.f;

        // Solve using Cardano's method
        float D = q * q + p * p * p;

        if (almost_zero (D))
        {
            if (almost_zero (q)) {
                s[0] = 0.f;
            } else {
                float u = std::cbrt (-q);
                s[0] = 2 * u;
                s[1] = -u;
            }
        } else if (D < 0) {
            float phi = 1 / 3.f * std::acos (-q / std::sqrt (-p * p * p));
            float t   = 2 * std::sqrt (-p);

            s[0] =  t * std::cos (phi);
            s[1] = -t * std::cos (phi + PI_f / 3.f);
            s[2] = -t * std::cos (phi - PI_f / 3.f);
        } else {
            float u = std::cbrt (std::sqrt (D) + std::fabs (q));
            if (q > 0.f)
                s[0] = -u + p / u;
            else
                s[0] =  u - p / u;
        }

        // Resubstitute a / 3 from above
        float sub = a / 3.f;
        for (auto &i: s)
            i -= sub;
        return s;
    }
} }
