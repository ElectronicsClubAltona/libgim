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

#include "bezier.hpp"

#include "debug.hpp"
#include "polynomial.hpp"

#include <algorithm>
#include <iterator>


//-----------------------------------------------------------------------------
template <size_t S>
util::bezier<S>::bezier (const util::point2f (&_points)[S+1])
{
    std::copy (_points, _points + S + 1, m_points);
}


//-----------------------------------------------------------------------------
namespace util {
    template <>
    point2f
    bezier<1>::eval (float t) const
    {
        CHECK_GE (t, 0);
        CHECK_LE (t, 1);

        auto v0 = (1 - t) * m_points[0];
        auto v1 =      t  * m_points[1];

        return {
            v0.x + v1.x,
            v0.y + v1.y
        };
    }
}


//-----------------------------------------------------------------------------
namespace util {
    template <>
    point2f
    bezier<2>::eval (float t) const
    {
        CHECK_GE (t, 0);
        CHECK_LE (t, 1);

        auto v0 =    pow2 (1 - t) * m_points[0];
        auto v1 = 2 * (1 - t) * t * m_points[1];
        auto v2 =        pow2 (t) * m_points[2];

        return {
            v0.x + v1.x + v2.x,
            v0.y + v1.y + v2.y
        };
    }
}


//-----------------------------------------------------------------------------
namespace util {
    template <>
    point2f
    bezier<3>::eval (float t) const
    {
        CHECK_GE (t, 0);
        CHECK_LE (t, 1);

        auto v0 = pow (1 - t, 3) * m_points[0];
        auto v1 = 3 * pow2 (1 - t) * t * m_points[1];
        auto v2 = 3 * pow2 (1 - t) * t * m_points[2];
        auto v3 = pow (t, 3) * m_points[3];

        return {
            v0.x + v1.x + v2.x + v3.x,
            v0.y + v1.y + v2.y + v3.y
        };
    }
}


//-----------------------------------------------------------------------------
namespace util {
    template <>
    float
    bezier<1>::distance (util::point2f target) const
    {
        auto v = m_points[1] - m_points[0];
        auto w = target - m_points[0];

        auto c1 = dot (w, v);
        if (c1 <= 0)
            return m_points[0].distance (target);

        auto c2 = dot (v, v);
        if (c2 <= c1)
            return m_points[1].distance (target);

        auto b = c1 / c2;
        auto p = m_points[0] + b * v;

        return p.distance (target);
    }
}


//-----------------------------------------------------------------------------
namespace util {
    // TODO: use a more reliable method like [Xiao-Dia Chen 2010]
    template <>
    float
    bezier<2>::distance (util::point2f target) const
    {
        // Using procedure from: http://blog.gludion.com/2009/08/distance-to-quadratic-bezier-curve.html
        auto p0 = m_points[0];
        auto p1 = m_points[1];
        auto p2 = m_points[2];

        // Parametric form: P(t) = (1-t)^2*P0 + 2t(1-t)P1 + t^2*P2
        //
        // Derivative: dP/dt = -2(1-t)P0 + 2(1-2t)P1 + 2tP2
        //                   = 2(A+Bt), A=(P1-P0), B=(P2-P1-A)
        //
        auto A = p1 - p0;
        auto B = p2 - p1 - A;

        // Make: dot(target, dP/dt) == 0
        //       dot (M - P(t), A+Bt) == 0
        //
        // Solve: at^3 + bt^2 + ct + d,
        //        a = B^2,
        //        b = 3A.B,
        //        c = 2A^2+M'.B,
        //        d = M'.A,
        //        M' = P0-M

        const auto M  = target;
        const auto M_ = p0 - M;

        //float a = dot (B, B);
        //float b = 3.f * dot (A, B);
        //float c = 2.f * dot (A, A) + dot (M_, B);
        //float d = dot (M_, A);

        const util::vector2f p102 = {
            2 * p1.x - p0.x - p2.x,
            2 * p1.y - p0.y - p2.y
        };

        const float a = dot (B, 2.f * p102);
        const float b = dot (B, 4.f * (p0 - p1)) + dot (A, 2.f * p102);
        const float c = dot (B, 2.f * (M - p0)) + dot (A, 4.f * (p0 - p1));
        const float d = dot (A, 2.f * (M - p0));

        auto solutions = util::polynomial::solve<3> ({a, b, c, d});

        float dist = std::numeric_limits<float>::infinity ();

        for (auto t: solutions) {
            if (std::isnan (t))
                continue;

            if (t <= 0)
                dist = min (dist, p0.distance (target));
            else if (t > 1)
                dist = min (p2.distance (target));
            else {
                auto p = eval (t);
                dist = min (dist, p.distance (target));
            }
        }

        return dist;
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

    float d_l = p_l.distance (target);
    float d_r = p_r.distance (target);

    if (d_l < d) { return refine_cubic (b, target, t_l, d_l, p); }
    if (d_r < d) { return refine_cubic (b, target, t_r, d_r, p); }

    return refine_cubic (b, target, t, d, p / 2);
}


//-----------------------------------------------------------------------------
namespace util {
    template <>
    float
    bezier<3>::distance (util::point2f target) const
    {
        static constexpr size_t SUBDIV = 32;
        std::array<util::point2f, SUBDIV> lookup;

        for (size_t i = 0; i < SUBDIV; ++i)
            lookup[i] = eval (i / float (SUBDIV - 1));

        size_t best = 0;
        for (size_t i = 1; i < lookup.size (); ++i)
            if (lookup[i].distance2 (target) < lookup[best].distance2 (target))
                best = i;

        return refine_cubic (*this,
                               target,
                               best / float (SUBDIV - 1),
                               lookup[best].distance (target),
                               1.f / SUBDIV);
    }
}


//-----------------------------------------------------------------------------
template <size_t S>
util::point2f&
util::bezier<S>::operator[] (size_t idx)
{
    CHECK_LE (idx, S);

    return m_points[idx];
}


//-----------------------------------------------------------------------------
template <size_t S>
const util::point2f&
util::bezier<S>::operator[] (size_t idx) const
{
    CHECK_LE (idx, S);

    return m_points[idx];
}


//-----------------------------------------------------------------------------
template <size_t S>
std::ostream&
util::operator<< (std::ostream &os, const bezier<S> &b)
{
    os << b[0];
    for (size_t i = 1; i < S+1; ++i)
        os << ", " << b[i];

    return os;
}


//-----------------------------------------------------------------------------
#define INSTANTIATE(S)                                                      \
template class util::bezier<S>;                                             \
template std::ostream& util::operator<< (std::ostream&, const bezier<S>&);

INSTANTIATE(1)
INSTANTIATE(2)
INSTANTIATE(3)
