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
 * Copyright 2015-2016 Danny Robson <danny@nerdcruft.net>
 */

#include "./bezier.hpp"

#include "./polynomial.hpp"
#include "./coord/iostream.hpp"


///////////////////////////////////////////////////////////////////////////////
namespace util {
    template <>
    point2f
    bezier<2>::eval (float t) const
    {
        CHECK_LIMIT (t, 0, 1);

        const auto &P0 = m_coeffs[0];
        const auto &P1 = m_coeffs[1];
        const auto &P2 = m_coeffs[2];

        return (
                (1 - t) * (1 - t) * P0 +
            2 * (1 - t) *      t  * P1 +
                     t  *      t  * P2
        ).as<util::point> ();
    }
}


//-----------------------------------------------------------------------------
namespace util {
    template <>
    std::array<util::vector2f,3>
    bezier<2>::coeffs (void) const
    {
        auto &v = m_coeffs;

        return {
            +1.f * v[2] -2.f * v[1] + 1.f * v[0],
            -2.f * v[2] +2.f * v[1],
            +1.f * v[2]
        };
    }
}


///////////////////////////////////////////////////////////////////////////////
namespace util {
    template <>
    util::vector2f
    bezier<2>::d1 (const float t) const noexcept
    {
        CHECK_LIMIT (t, 0, 1);

        const auto &P0 = m_coeffs[0];
        const auto &P1 = m_coeffs[1];
        const auto &P2 = m_coeffs[2];

        return 2 * (1 - t) * (P1 - P0) +
               2 *      t  * (P2 - P1);
    }
}


///////////////////////////////////////////////////////////////////////////////
namespace util {
    template <>
    sdot_t
    bezier<2>::sdot (point2f q) const noexcept
    {
        // setup inter-point vectors
        const auto ab = m_points[1] - m_points[0];
        const auto bc = m_points[2] - m_points[1];
        const auto qa = m_points[0] - q;
        const auto qb = m_points[1] - q;
        const auto qc = m_points[2] - q;

        // setup variables we want to minimise
        float d = std::numeric_limits<float>::infinity ();
        float t = std::numeric_limits<float>::quiet_NaN ();

        // distance from A
        const auto d_a = sign (cross (ab, qa)) * norm2 (qa);
        if (abs (d_a) < abs (d)) {
            d = d_a;
            t = -dot (ab, qa) / norm2 (ab);
        }

        // distance from B
        const auto d_b = sign (cross (bc, qc)) * norm2 (qc);
        if (abs (d_b) < abs (d)) {
            d = d_b;
            t = -dot (bc, qb) / norm2 (bc);
        }

        // Using procedure from: http://blog.gludion.com/2009/08/distance-to-quadratic-bezier-curve.html
        //
        // Parametric form: P(t) = (1-t)^2*P0 + 2t(1-t)P1 + t^2*P2
        //
        // Derivative: dP/dt = -2(1-t)P0 + 2(1-2t)P1 + 2tP2
        //                   = 2(A+Bt), A=(P1-P0), B=(P2-P1-A)
        //
        const auto &p0 = m_points[0];
        const auto &p1 = m_points[1];
        const auto &p2 = m_points[2];

        const auto A = p1 - p0;
        const auto B = p2 - p1 - A;

        // Make: dot (q, dP/dt) == 0
        //       dot (M - P(t), A + Bt) == 0
        //
        // Solve: at^3 + bt^2 + ct + d,
        //        a  = B^2,
        //        b  = 3A.B,
        //        c  = 2A^2+M'.B,
        //        d  = M'.A,
        //        M' = P0-M

        const auto M  = q;
        const auto M_ = p0 - M;

        const std::array<float,4>
        poly = {
                dot (B,  B),
            3 * dot (A,  B),
            2 * dot (A,  A) + dot (M_, B),
                dot (M_, A),
        };

        // test at polynomial minima
        for (const auto r: polynomial::roots<3> (poly)) {
            // bail if we have fewer roots than expected
            if (std::isnan (r))
                break;

            // ignore if this root is off the curve
            if (r < 0 || r > 1)
                continue;

            const auto qe = eval (r) - q;

            const auto d_e = sign (cross (ab, qe)) * norm2 (qe);
            if (abs (d_e) <= abs (d)) {
                d = d_e;
                t = r;
            }
        }

        // calculate the angles from the point to the endpoints if needed
        d = sign (d) * std::sqrt (abs (d));

        if (t >= 0 && t <= 1)
            return { d, 0 };
        if (t < 0) {
            return { d, abs (dot (normalised (ab), normalised (qa))) };
        } else
            return { d, abs (dot (normalised (bc), normalised (qc))) };
    }
}


///////////////////////////////////////////////////////////////////////////////
namespace util {
    template <>
    float
    bezier<2>::distance (util::point2f q) const noexcept
    {
        return abs (sdot (q).distance);
    }
}


