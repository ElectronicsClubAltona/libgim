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
 * Copyright 2011-2012 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_MATRIX_HPP
#define __UTIL_MATRIX_HPP

#include "point.hpp"

#include <iostream>

namespace util {
    struct matrix {
        double values[4][4];

        void scale     (double x, double y, double z);
        void translate (double x, double y, double z);

        matrix  inverse (void) const;
        matrix& invert  (void);

        matrix operator* (const matrix&) const;

        point<3> to_local  (const point<3> &p) const;
        point<3> to_global (const point<3> &p) const;

        bool is_affine (void) const;

        static const matrix IDENTITY;
        static const matrix ZEROES;
    };
}

std::ostream& operator<< (std::ostream&, const util::matrix&);

#endif
