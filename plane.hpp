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

#ifndef __UTIL_PLANE_HPP
#define __UTIL_PLANE_HPP

#include "point.hpp"
#include "vector.hpp"

namespace util {
    template <size_t S, typename T>
    struct plane {
        plane (util::point<S,T> p,
               util::vector<S,T> n);

        util::point<S,T> p;
        util::vector<S,T> n;
    };


    typedef plane<2,float> plane2f;
    typedef plane<3,float> plane3f;
}

#endif
