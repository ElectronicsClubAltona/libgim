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


#ifndef __UTIL_AABB_HPP
#define __UTIL_AABB_HPP

#include "point.hpp"
#include "extent.hpp"

#include <cstdint>
#include <iostream>

namespace util {
    template <size_t S, typename T>
    struct AABB {
        AABB () = default;
        AABB (point<S,T>, point<S,T>);

        extent<S,T> magnitude (void) const;

        bool overlaps (point<S,T>) const;

        void expand (point<S,T>);

        AABB<S,T> operator+ (vector<S,T>) const;
        AABB<S,T> operator- (vector<S,T>) const;

        point<S,T> p0;
        point<S,T> p1;
    };

    typedef AABB<2,float> AABB2f;
    typedef AABB<2,size_t> AABB2u;
    typedef AABB<2,intmax_t> AABB2i;

    typedef AABB<3,float> AABB3f;
    typedef AABB<3,size_t> AABB3u;
    typedef AABB<3,intmax_t> AABB3i;

    template <size_t S, typename T>
    std::ostream& operator<< (std::ostream&, AABB<S,T>);
}

#endif
