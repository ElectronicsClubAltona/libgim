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

#include "debug.hpp"
#include "polynomial.hpp"
#include "stream.hpp"
#include "coord/iostream.hpp"

#include <algorithm>
#include <iterator>


///////////////////////////////////////////////////////////////////////////////
template <size_t N>
util::bezier<N>::bezier (const util::point2f (&_points)[N+1])
{
    std::copy (_points, _points + N + 1, m_points);
}


//-----------------------------------------------------------------------------
// XXX: If the line is co-linear we'll have no solutions. But we return 1
// anyway as this function is used to find any point that intersects as part
// of other more comprehensive tests.
template <size_t N>
size_t
util::bezier<N>::intersections (point2f p0, point2f p1) const
{
    float A = p1.y - p0.y;              // A = y2 - y1
    float B = p0.x - p1.x;              // B = x1 - x2
    float C = p0.x * (p0.y - p1.y) +    // C = x1 (y1 - y2) + y1 (x2 - x1)
              p0.y * (p1.x - p0.x);

    // Build the intersection polynomial
    const std::array<vector2f,N+1> bcoeff = coeffs ();

    std::array<float,N+1> pcoeff;
    for (size_t i = 0; i < pcoeff.size (); ++i)
        pcoeff[i] = A * bcoeff[i].x + B * bcoeff[i].y;
    pcoeff.back () += C;

    const auto r = polynomial::roots<N> (pcoeff);

    // The curve and line are colinear
    if (std::all_of (r.begin (), r.end (), [] (auto i) { return std::isnan (i); }))
        return 1;

    size_t count = 0;
    for (size_t i = 0; i < N; ++i) {
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
template <size_t N>
util::region2f
util::bezier<N>::region (void) const
{
    float x0 = m_points[0].x;
    float y0 = m_points[0].y;

    float x1 = x0;
    float y1 = y0;

    for (size_t i = 1; i < N+1; ++i) {
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
template <size_t N>
util::point2f&
util::bezier<N>::operator[] (size_t idx)
{
    CHECK_LE (idx, N);

    return m_points[idx];
}


//-----------------------------------------------------------------------------
template <size_t N>
const util::point2f&
util::bezier<N>::operator[] (size_t idx) const
{
    CHECK_LE (idx, N);

    return m_points[idx];
}


///////////////////////////////////////////////////////////////////////////////
template <size_t N>
const util::point2f*
util::bezier<N>::begin (void) const
{
    return std::cbegin (m_points);
}


//-----------------------------------------------------------------------------
template <size_t N>
const util::point2f*
util::bezier<N>::end (void) const
{
    return std::cend (m_points);
}


//-----------------------------------------------------------------------------
template <size_t N>
const util::point2f*
util::bezier<N>::cbegin (void) const
{
    return std::cbegin (m_points);
}


//-----------------------------------------------------------------------------
template <size_t N>
const util::point2f*
util::bezier<N>::cend (void) const
{
    return std::cend (m_points);
}


///////////////////////////////////////////////////////////////////////////////
template <size_t N>
std::ostream&
util::operator<< (std::ostream &os, const bezier<N> &b)
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
#define INSTANTIATE(N)                                                      \
template class util::bezier<N>;                                             \
template std::ostream& util::operator<< (std::ostream&, const bezier<N>&);

INSTANTIATE(1)
INSTANTIATE(2)
INSTANTIATE(3)
