/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

        matrix& transpose  (void);
        matrix  transposed (void) const;

        matrix<T>  inverse (void) const;
        matrix<T>& invert  (void);
        matrix<T>  inverse_affine (void) const;
        matrix<T>& invert_affine  (void);

        T det (void) const;

        matrix<T>   operator* (const matrix<T>&) const;
        matrix<T>&  operator*=(const matrix<T>&);

        vector<3,T> operator* (vector<3,T>) const;
        point<3,T>  operator* (point<3,T>) const;

        vector<4,T> operator* (const vector<4,T>&) const;
        point<4,T>  operator* (const point<4,T> &) const;

        matrix<T>  operator*  (T) const;
        matrix<T>& operator*= (T);
        matrix<T>  operator/  (T) const;
        matrix<T>& operator/= (T);

        bool operator== (const matrix<T>&) const;

        bool is_affine (void) const;

        template <typename U>
        matrix<U> cast (void) const;

        // Perspective matrices
        static matrix<T> ortho   (T left, T right, T bottom, T top, T near, T far);
        static matrix<T> ortho2D (T left, T right, T bottom, T top);
        static matrix<T> perspective (T fov, T aspect, T near, T far);
        static matrix<T> look_at (point<3,T> eye, point<3,T> centre, vector<3,T> up);

        // Affine matrices
        static matrix<T> translate (util::vector<2,T>);
        static matrix<T> translate (util::vector<3,T>);
        static matrix<T> scale     (util::vector<3,T>);
        static matrix<T> scale     (T);
        static matrix<T> rotate    (T angle, util::vector<3,T> about);

        // Constant matrices
        static const matrix<T> IDENTITY;
        static const matrix<T> ZEROES;
    };

    template <typename T> using matrix4 = matrix<T>;

    typedef matrix<float> matrixf;
    typedef matrix<float> matrix4f;

    template <typename T>
    std::ostream& operator<< (std::ostream&, const matrix<T>&);
}

#include "matrix.ipp"

#endif
