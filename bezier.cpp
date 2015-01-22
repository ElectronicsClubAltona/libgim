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
template class util::bezier<2>;
template class util::bezier<3>;
template class util::bezier<4>;
