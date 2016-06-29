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

#include "./debug.hpp"
#include "./polynomial.hpp"
#include "./stream.hpp"
#include "./coord/iostream.hpp"

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
        auto v2 =    pow2 (t)     * m_points[2];

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
    float
    bezier<1>::distance (util::point2f target) const
    {
        auto v = m_points[1] - m_points[0];
        auto w = target - m_points[0];

        auto c1 = dot (w, v);
        if (c1 <= 0)
            return util::distance (target, m_points[0]);

        auto c2 = dot (v, v);
        if (c2 <= c1)
            return util::distance (target, m_points[1]);

        auto b = c1 / c2;
        auto p = m_points[0] + b * v;

        return util::distance (target, p);
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


//-----------------------------------------------------------------------------
namespace util {
    template <>
    std::array<util::vector2f,2>
    bezier<1>::coeffs (void) const
    {
        auto &v = m_coeffs;

        return {
            -1.f * v[1] + 1.f * v[0],
            +1.f * v[1],
        };
    }
}


//-----------------------------------------------------------------------------
// XXX: If the line is co-linear we'll have no solutions. But we return 1
// anyway as this function is used to find any point that intersects as part
// of other more comprehensive tests.
template <size_t S>
size_t
util::bezier<S>::intersections (point2f p0, point2f p1) const
{
    float A = p1.y - p0.y;              // A = y2 - y1
    float B = p0.x - p1.x;              // B = x1 - x2
    float C = p0.x * (p0.y - p1.y) +    // C = x1 (y1 - y2) + y1 (x2 - x1)
              p0.y * (p1.x - p0.x);

    // Build the intersection polynomial
    const std::array<vector2f,S+1> bcoeff = coeffs ();

    std::array<float,S+1> pcoeff;
    for (size_t i = 0; i < pcoeff.size (); ++i)
        pcoeff[i] = A * bcoeff[i].x + B * bcoeff[i].y;
    pcoeff.back () += C;

    const auto r = polynomial::roots<S> (pcoeff);

    // The curve and line are colinear
    if (std::all_of (r.begin (), r.end (), [] (auto i) { return std::isnan (i); }))
        return 1;

    size_t count = 0;
    for (size_t i = 0; i < S; ++i) {
        // Ensure the solutions are on the curve
        const auto t = r[i];
        if (std::isnan (t))
            break;

        if (t < 0.f || t > 1.f)
            continue;

        // Find the line's intersection point
        const util::vector2f q = polynomial::eval (bcoeff, t);

        const auto s = almost_equal (p0.x, p1.x)   ?
                          (q.y-p0.y) / (p1.y-p0.y) :
                          (q.x-p0.x) / (p1.x-p0.x) ; // vertical

        // Check if the point is on the line
        if (s >= 0.f && s <= 1.f)
            ++count;
    }

    return count;
}


//-----------------------------------------------------------------------------
namespace util {
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

        float a = dot (B, B);
        float b = 3.f * dot (A, B);
        float c = 2.f * dot (A, A) + dot (M_, B);
        float d = dot (M_, A);

        // We have our cubic, so pass off to the solver
        auto solutions = util::polynomial::roots<3> ({a, b, c, d});

        // Find the smallest distance and return
        float dist = std::numeric_limits<float>::infinity ();

        for (auto t: solutions) {
            if (std::isnan (t))
                continue;

            if (t <= 0)
                dist = min (dist, util::distance (target, p0));
            else if (t > 1)
                dist = min (util::distance (target, p2));
            else {
                auto p = eval (t);
                dist = min (dist, util::distance (target, p));
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
    bezier<3>::distance (util::point2f target) const
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


//-----------------------------------------------------------------------------
template <size_t S>
util::region2f
util::bezier<S>::region (void) const
{
    float x0 = m_points[0].x;
    float y0 = m_points[0].y;

    float x1 = x0;
    float y1 = y0;

    for (size_t i = 1; i < S+1; ++i) {
        x0 = min (x0, m_points[i].x);
        y0 = min (y0, m_points[i].y);

        x1 = max (x1, m_points[i].x);
        y1 = max (y1, m_points[i].y);
    }

    util::point2f p0 { x0, y0 };
    util::point2f p1 { x1, y1 };

    return { p0, p1 };
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


///////////////////////////////////////////////////////////////////////////////
template <size_t S>
const util::point2f*
util::bezier<S>::begin (void) const
{
    return std::cbegin (m_points);
}


//-----------------------------------------------------------------------------
template <size_t S>
const util::point2f*
util::bezier<S>::end (void) const
{
    return std::cend (m_points);
}


//-----------------------------------------------------------------------------
template <size_t S>
const util::point2f*
util::bezier<S>::cbegin (void) const
{
    return std::cbegin (m_points);
}


//-----------------------------------------------------------------------------
template <size_t S>
const util::point2f*
util::bezier<S>::cend (void) const
{
    return std::cend (m_points);
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S>
std::ostream&
util::operator<< (std::ostream &os, const bezier<S> &b)
{
    using value_type = decltype(*b.cbegin());

    os << "[";
    std::transform (std::cbegin (b),
                    std::cend   (b),
                    infix_iterator<value_type> (os, ", "),
                    [] (auto i) { return +i; });
    os << "]";

    return os;
}


///////////////////////////////////////////////////////////////////////////////
#define INSTANTIATE(S)                                                      \
template class util::bezier<S>;                                             \
template std::ostream& util::operator<< (std::ostream&, const bezier<S>&);

INSTANTIATE(1)
INSTANTIATE(2)
INSTANTIATE(3)
