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

#ifndef __UTIL_BEZIER_HPP
#define __UTIL_BEZIER_HPP

#include "point.hpp"

#include <iostream>

namespace util {
    template <size_t S>
    class bezier {
    public:
        bezier (const util::point2f (&)[S+1]);

        point2f eval (float t) const;

        // Calculate the expanded polynomial coeffecients in terms of t
        std::array<vector2f,S+1>
        coeffs (void) const;

        size_t intersections (point2f from, point2f to) const;

        float distance (point2f) const;

        point2f& operator[] (size_t idx);
        const point2f& operator[] (size_t idx) const;

    private:
        // HACK: allow easy access to component-wise arithmetic using
        // vector2f rather than point2f in the implementation.
        union {
            point2f  m_points[S+1];
            vector2f m_coeffs[S+1];
        };
    };

    template <size_t S>
    std::ostream& operator<< (std::ostream&, const bezier<S>&);
}

#endif
