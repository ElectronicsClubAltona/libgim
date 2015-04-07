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

#include "coord.hpp"
#include "vector.hpp"
#include <iostream>


namespace util {
    /**
     * A pure two-dimensional size, without positioning
     */
    template <size_t S, typename T>
    struct extent : public coord::base<S,T,coord::whd>
    {
        using coord::base<S,T,coord::whd>::base;

        extent () = default;
        extent (vector<S,T>);

        T    area  (void) const;
        T diameter (void) const;

        template <typename U = float>
        U aspect (void) const;

        extent expanded (vector<S,T>) const;
        extent expanded (T) const;

        bool empty (void) const;

        template <typename U>
        extent<S,U> cast (void) const;

        static const extent MAX;
        static const extent MIN;
    };

    typedef extent<2,intmax_t> extent2i;
    typedef extent<2,size_t> extent2u;
    typedef extent<2,float> extent2f;
    typedef extent<2,double> extent2d;

    template <size_t S, typename T>
    std::ostream& operator<< (std::ostream&, util::extent<S,T>);
}


#include "extent.ipp"

#endif
