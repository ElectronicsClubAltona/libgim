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

#include <array>
#include <iostream>


///////////////////////////////////////////////////////////////////////////////
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


///////////////////////////////////////////////////////////////////////////////
constexpr
util::vector2f
orthonormal (util::vector2f v)
{
    const auto len = norm (v);
    CHECK_NEZ (len);
    return util::vector2f { -v.y / len, v.x / len };
}


//-----------------------------------------------------------------------------
namespace util {
    template <>
    float
    bezier<1>::closest (util::point2f q) const noexcept
    {
        const auto ab = m_points[1] - m_points[0];
        const auto aq =           q - m_points[0];

        return dot (aq, ab) / dot (ab, ab);
    }
}


//-----------------------------------------------------------------------------
namespace util {
    template <>
    float
    bezier<1>::distance (util::point2f q) const noexcept
    {
        const auto ab = m_points[1] - m_points[0];
        const auto  t = limit (closest (q), 0, 1);
        const auto  p = m_points[0] + t * ab;

        return util::distance (q, p);
    }
}


//-----------------------------------------------------------------------------
namespace util {
    template <>
    sdot_t
    bezier<1>::sdot (const point2f q) const noexcept
    {
        // find the closest parameter 't' to the point 'q' for the parametric line
        const auto qa = m_points[0] - q;
        const auto ab = m_points[1] - m_points[0];
        const auto  t = closest (q);

        // find the vector to, and distance to, the nearest endpoint 'e'
        const auto qe = m_points[t > 0.5] - q;
        const auto d_e = norm (qe);

        // if we're on the segment return the distance to the segment
        if (t >= 0 && t <= 1) {
            const auto ortho = util::vector2f { -ab.y, ab.x } / norm (ab);
            const auto d = dot (ortho, qa);

            // not _entirely_ sure why we need this condition
            if (abs (d) <= d_e) {
                return { d, 0 };
            }
        }

        // return the distance and angle to the endpoint
        return {
            sign (cross (ab, qa)) * d_e,
            abs (
                dot (normalised (ab), normalised (qe))
            )
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
