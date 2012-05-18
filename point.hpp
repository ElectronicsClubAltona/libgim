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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_POINT_HPP
#define __UTIL_POINT_HPP

#include "vector.hpp"
#include "detail/coord.hpp"

#include <array>
#include <initializer_list>
#include <iostream>

namespace util {
    /// An n-dimensional position in space.
    template <size_t S>
    struct point : public detail::coord_data<S> {
        static_assert (S > 0, "point dimensions must be strictly positive.");

        point ();

        template <typename... T>
        point (T ...t): detail::coord_data<S> {std::forward<T> (t)...} { ; }

        double distance  (const point &) const;
        double distance2 (const point &) const;
        double manhattan (const point &) const;

        point<S>& operator*= (double);
        point<S>  operator*  (double) const;
        point<S>  operator-  (const point<S>&) const;

        point<S>  operator+  (const util::vector<S>&) const;
        point<S>& operator+= (const util::vector<S>&);

        util::vector<S> to (const point<S>&) const;

        template <size_t D> point<D> redim (void);

        void sanity (void) const;
    };

    typedef point<2> point2;
    typedef point<3> point3;

    template <size_t S>
    std::ostream& operator<< (std::ostream&, const util::point<S>&);
}



#endif // __UTIL_POINT_HPP
