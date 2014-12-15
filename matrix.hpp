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
 * Copyright 2011-2014 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_MATRIX_HPP
#define __UTIL_MATRIX_HPP

#include "point.hpp"

#include <iostream>

namespace util {
    template <typename T>
    struct matrix {
        T values[4][4];

        static const size_t rows = 4;
        static const size_t cols = 4;

        void scale     (T x, T y, T z);
        void translate (T x, T y, T z);

        matrix& transpose  (void);
        matrix  transposed (void) const;

        matrix<T>  inverse (void) const;
        matrix<T>& invert  (void);
        matrix<T>  inverse_affine (void) const;
        matrix<T>& invert_affine  (void);

        T det (void) const;

        matrix<T>   operator* (const matrix<T>&) const;
        vector<4,T> operator* (const vector<4,T>&) const;

        matrix<T>& operator*= (T);
        matrix<T>  operator/  (T) const;
        matrix<T>& operator/= (T);

        bool operator== (const matrix<T>&) const;

        point<3,T> to_local  (const point<3,T> &p) const;
        point<3,T> to_global (const point<3,T> &p) const;

        bool is_affine (void) const;

        // Perspective matrices
        static matrix<T> ortho   (T left, T right, T bottom, T top, T near, T far);
        static matrix<T> ortho2D (T left, T right, T bottom, T top);
        static matrix<T> perspective (T fov, T aspect, T near, T far);
        static matrix<T> look_at (point<3,T> eye, point<3,T> centre, vector<3,T> up);

        // Affine matrices
        static matrix<T> translate (util::vector<3,T>);
        static matrix<T> scale     (util::vector<3,T>);
        static matrix<T> rotate    (util::vector<3,T> about, T angle);

        // Constant matrices
        static const matrix<T> IDENTITY;
        static const matrix<T> ZEROES;
    };

    typedef matrix<float> matrixf;

    template <typename T>
    std::ostream& operator<< (std::ostream&, const matrix<T>&);
}


#endif
