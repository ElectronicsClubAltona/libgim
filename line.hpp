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

#ifndef __UTIL_LINE_HPP
#define __UTIL_LINE_HPP

#include "point.hpp"
#include "vector.hpp"
#include "plane.hpp"
#include "aabb.hpp"

namespace util {
    template <size_t S, typename T>
    struct line {
        line (util::point<S,T> origin,
              util::vector<S,T> direction);

        T intersect (plane<S,T>) const;
        T intersect (AABB<S,T>) const;

        T closest (point<S,T>) const;

        util::point<S,T> at (T) const;

        util::point<S,T> p;
        util::vector<S,T> d;
    };


    typedef line<2,float> line2f;
    typedef line<3,float> line3f;
}

#endif
