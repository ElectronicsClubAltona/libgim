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

#include "bezier.hpp"

#include "coord/iostream.hpp"

#include <array>


///////////////////////////////////////////////////////////////////////////////
namespace util {
    template <>
    point2f
    bezier<3>::eval (float t) const
    {
        CHECK_GE (t, 0);
        CHECK_LE (t, 1);

        auto v0 =     pow  (1 - t, 3)  * m_points[0];
        auto v1 = 3 * pow2 (1 - t) * t * m_points[1];
        auto v2 = 3 * pow2 (1 - t) * t * m_points[2];
        auto v3 =     pow  (t, 3)      * m_points[3];

        return {
            v0.x + v1.x + v2.x + v3.x,
            v0.y + v1.y + v2.y + v3.y
        };
    }
}


//-----------------------------------------------------------------------------
namespace util {
    template <>
    std::array<util::vector2f,4>
    bezier<3>::coeffs (void) const
    {
        const auto &v = m_coeffs;

        return {
            -1.f * v[0] +3.f * v[1] -3.f * v[2] +1.f * v[3],
             3.f * v[0] -6.f * v[1] +3.f * v[2],
            -3.f * v[0] +3.f * v[1],
             1.f * v[0]
        };
    }
}


//-----------------------------------------------------------------------------
float refine_cubic (util::bezier<3> b,
                    util::point2f target,
                    float t,
                    float d,
                    float p)
{
    // TODO: use an iteration of newton before handing back
    if (p < 0.00001) {
        return t;
    }

    float t_l = std::max (0.f, t - p);
    float t_r = std::min (1.f, t + p);

    util::point2f p_l = b.eval (t_l);
    util::point2f p_r = b.eval (t_r);

    float d_l = util::distance (target, p_l);
    float d_r = util::distance (target, p_r);

    if (d_l < d) { return refine_cubic (b, target, t_l, d_l, p); }
    if (d_r < d) { return refine_cubic (b, target, t_r, d_r, p); }

    return refine_cubic (b, target, t, d, p / 2);
}


//-----------------------------------------------------------------------------
namespace util {
    // TODO: use a more reliable method like [Xiao-Dia Chen 2010]
    template <>
    float
    bezier<3>::distance (util::point2f target) const noexcept
    {
        static constexpr size_t SUBDIV = 32;
        std::array<util::point2f, SUBDIV> lookup;

        for (size_t i = 0; i < SUBDIV; ++i)
            lookup[i] = eval (i / float (SUBDIV - 1));

        size_t best = 0;
        for (size_t i = 1; i < lookup.size (); ++i) {
            auto d_i = util::distance2 (target, lookup[i]);
            auto d_b = util::distance2 (target, lookup[best]);

            if (d_i < d_b)
                best = i;
        }

        return refine_cubic (*this,
                               target,
                               best / float (SUBDIV - 1),
                               util::distance (target, lookup[best]),
                               1.f / SUBDIV);
    }
}


///////////////////////////////////////////////////////////////////////////////
namespace util {
    template <>
    util::vector2f
    bezier<3>::tangent (const float t) const
    {
        CHECK_LIMIT (t, 0, 1);

        return mix (
            mix (m_coeffs[1] - m_coeffs[0], m_coeffs[2] - m_coeffs[1], t),
            mix (m_coeffs[2] - m_coeffs[1], m_coeffs[3] - m_coeffs[2], t),
            t
        );
    }
}


//-----------------------------------------------------------------------------
namespace util {
    template <>
    util::vector2f
    bezier<3>::d1 (const float t) const noexcept
    {
        const auto &P0 = m_points[0];
        const auto &P1 = m_points[1];
        const auto &P2 = m_points[2];
        const auto &P3 = m_points[3];

        return 3 * (1 - t) * (1 - t) * (P1 - P0) +
               6 * (1 - t) *      t  * (P2 - P1) +
               3 *      t  *      t  * (P3 - P2);
    }
}


//-----------------------------------------------------------------------------
namespace util {
    template <>
    util::vector2f
    bezier<3>::d2 (const float t) const noexcept
    {
        const auto &P0 = m_points[0];
        const auto &P1 = m_points[1];
        const auto &P2 = m_points[2];
        const auto &P3 = m_points[3];

        return 6 * (1 - t) * (P2 - P1 + P0 - P1) +
               6 *      t  * (P3 - P2 + P1 - P2);
    }
}


///////////////////////////////////////////////////////////////////////////////
namespace util {
    template <>
    sdot_t
    bezier<3>::sdot (point2f src) const noexcept
    {
        const auto ab = m_points[1] - m_points[0];
        const auto cd = m_points[3] - m_points[2];
        const auto qa = m_points[0] - src;
        const auto qd = m_points[3] - src;

        // setup variables for minimisation
        float d = std::numeric_limits<float>::infinity ();
        float t = std::numeric_limits<float>::quiet_NaN ();

        // distance from A
        const auto d_a = util::sign (cross (ab, qa)) * norm (qa);
        if (abs (d_a) < abs (d)) {
            d = d_a;
            t = -dot (ab, qa) / norm2 (ab);
        }

        // distance from D
        const auto d_d = util::sign (cross (cd, qd)) * norm (qd);
        if (abs (d_d) < abs (d)) {
            d = d_d;
            t = -dot (cd, qd) / norm2 (cd);
        }

        // Iterative minimum distance search
        static constexpr int SUBDIVISIONS = 4;
        static constexpr int REFINEMENTS  = 4;

        for (int i = 0; i <= SUBDIVISIONS; ++i) {
            auto r = float (i) / SUBDIVISIONS;

            for (int step = 0; ; ++step) {
                const auto qp = eval (r) - src;

                const auto d_p = sign (cross (tangent (r), qp)) * norm (qp);
                if (abs (d_p) < abs (d)) {
                    d = d_p;
                    t = r;
                }

                if (step == REFINEMENTS)
                    break;

                // converge a little using newton's method
                const auto d1_ = d1 (r);
                const auto d2_ = d2 (r);
                r -= dot (qp, d1_) / (dot (d1_, d1_) + dot (qp, d2_));

                // bail if it looks like we're going to escape the curve
                if (r < 0 || r > 1)
                    break;
            }
        }

        if (t >= 0 && t <= 1)
            return { d, 0 };
        if (t < 0)
            return { d, abs (dot (normalised (ab), normalised (qa))) };
        else
            return { d, abs (dot (normalised (cd), normalised (qd))) };
    }
}
