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
 * Copyright 2010-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_EXTENT_HPP
#define __UTIL_EXTENT_HPP

#include "vector.hpp"

#include <iostream>


namespace util {
    /**
     * A pure two-dimensional size, without positioning
     */
    template <typename T>
    struct extent {
        T w, h;

        extent (const T  _width, const T  _height);
        extent (T);
        extent (vector<2,T>);
        extent (const extent&);
        extent& operator= (const extent&);
        extent () = default;

        T    area  (void) const;
        T diameter (void) const;

        extent<T> expanded (util::vector<2,T>) const;
        extent<T> expanded (T) const;

        float aspect (void) const;

        bool empty (void) const;

        bool operator ==(const extent& rhs) const;
        bool operator !=(const extent& rhs) const
            { return !(*this == rhs); }

        template <typename U>
        extent<U> cast (void) const;

        static const extent<T> MAX;
        static const extent<T> MIN;
    };

    typedef extent<int> extent2i;
    typedef extent<size_t> extent2u;
    typedef extent<float> extent2f;

    template <typename T>
    std::ostream& operator<< (std::ostream&, util::extent<T>);
}


#include "extent.ipp"

#endif
