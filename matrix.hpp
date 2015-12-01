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
 * Copyright 2011-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_MATRIX_HPP
#define __UTIL_MATRIX_HPP

#include "point.hpp"
#include "range.hpp"

#include <iostream>

namespace util {
    template <size_t S, typename T>
    struct matrix {
        T values[S][S];

        static const size_t rows = S;
        static const size_t cols = S;

        T* operator[] (size_t);
        const T* operator[] (size_t) const;

        matrix& transpose  (void);
        matrix  transposed (void) const;

        T determinant (void) const;

        matrix  inverse (void) const;
        matrix& invert  (void);
        matrix  inverse_affine (void) const;
        matrix& invert_affine  (void);

        T det (void) const;

        matrix   operator* (const matrix&) const;
        matrix&  operator*=(const matrix&);

        vector<S,T> operator* (const vector<S,T>&) const;
        point<S,T>  operator* (const point<S,T> &) const;

        matrix  operator*  (T) const;
        matrix& operator*= (T);
        matrix  operator/  (T) const;
        matrix& operator/= (T);

        bool operator== (const matrix&) const;

        bool is_affine (void) const;

        template <typename U>
        matrix<S,U> cast (void) const;

        // Perspective matrices
        static matrix<4,T> ortho   (T left, T right, T bottom, T top, T near, T far);
        static matrix<4,T> ortho2D (T left, T right, T bottom, T top);
        static matrix<4,T> perspective (T fov, T aspect, range<T> Z);
        static matrix<4,T> look_at (point<3,T> eye, point<3,T> centre, vector<3,T> up);

        // Affine matrices
        static matrix<4,T> translate (util::vector<2,T>);
        static matrix<4,T> translate (util::vector<3,T>);
        static matrix<4,T> scale     (util::vector<3,T>);
        static matrix<4,T> scale     (T);
        static matrix<4,T> rotate    (T angle, util::vector<3,T> about);

        // Constant matrices
        static const matrix IDENTITY;
        static const matrix ZEROES;
    };


    template <size_t S, typename T>
    T determinant (const matrix<S,T>&);

    template <size_t S, typename T>
    matrix<S,T>
    inverse (const matrix<S,T>&);

    template <typename T> using matrix3 = matrix<3,T>;
    template <typename T> using matrix4 = matrix<4,T>;

    template <size_t S> using matrixf = matrix<S,float>;
    template <size_t S> using matrixd = matrix<S,double>;

    typedef matrix<2,float> matrix2f;
    typedef matrix<2,double> matrix2d;

    typedef matrix<3,float> matrix3f;
    typedef matrix<3,double> matrix3d;

    typedef matrix<4,float> matrix4f;
    typedef matrix<4,double> matrix4d;

    template <size_t S, typename T>
    std::ostream& operator<< (std::ostream&, const matrix<S,T>&);
}

#include "matrix.ipp"

#endif
