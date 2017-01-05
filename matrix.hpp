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

#include <ostream>

namespace util {
    template <size_t S, typename T>
    struct matrix {
        T values[S][S];

        static constexpr size_t rows = S;
        static constexpr size_t cols = S;

        // index operators return a pointer into the data array so that
        // multidimensional array syntax can be used transparently on this
        // type.
        T* operator[] (size_t);
        const T* operator[] (size_t) const;

        T* data (void);
        const T* data (void) const;

        const T* begin (void) const;
        const T* end   (void) const;
        T* begin (void);
        T* end   (void);
        const T* cbegin (void) const;
        const T* cend   (void) const;

        matrix& transpose  (void);
        matrix  transposed (void) const;

        T determinant (void) const;

        matrix  inverse (void) const;
        matrix& invert  (void);
        matrix  inverse_affine (void) const;
        matrix& invert_affine  (void);

        matrix   operator* (const matrix&) const;
        matrix&  operator*=(const matrix&);

        vector<S,T> operator* (const vector<S,T>&) const;
        point<S,T>  operator* (const point<S,T> &) const;

        bool is_affine (void) const;

        template <typename U>
        matrix<S,U> cast (void) const;

        // Perspective matrices
        static matrix<4,T> ortho   (T left, T right, T bottom, T top, T near, T far);
        static matrix<4,T> ortho2D (T left, T right, T bottom, T top);
        static matrix<4,T> perspective (T fov, T aspect, range<T> Z);
        static matrix<4,T> look_at (point<3,T> eye, point<3,T> target, vector<3,T> up);

        // Affine matrices
        static matrix<4,T> translation (util::vector<2,T>);
        static matrix<4,T> translation (util::vector<3,T>);
        static matrix<4,T> scale       (util::vector<3,T>);
        static matrix<4,T> scale       (T);
        static matrix<4,T> rotation    (T angle, util::vector<3,T> about);

        // Constant matrices
        static constexpr matrix identity ();
        static constexpr matrix zeroes ();
    };


    ///////////////////////////////////////////////////////////////////////////
    // Convert an affine rotation matrix to euler angles.
    //
    // Results are undefined if the matrix is not purely a rotation matrix,
    // or if the dimension is not 3x3 or 4x4.
    template <size_t S, typename T>
    vector<3,T>
    to_euler (const matrix<S,T>&);


    ///////////////////////////////////////////////////////////////////////////
    // logical operations
    template <size_t S, typename T>
    constexpr
    bool
    operator== (const matrix<S,T>&, const matrix<S,T>&);


    template <size_t S, typename T>
    constexpr
    bool
    operator!= (const matrix<S,T>&, const matrix<S,T>&);


    ///////////////////////////////////////////////////////////////////////////
    // element operations
    template <size_t S, typename T>
    constexpr
    matrix<S,T>
    operator+ (const matrix<S,T>&, const matrix<S,T>&);

    template <size_t S, typename T>
    constexpr
    matrix<S,T>
    operator- (const matrix<S,T>&, const matrix<S,T>&);


    ///////////////////////////////////////////////////////////////////////////
    // scalar operations
    template <size_t S, typename T> constexpr matrix<S,T> operator* (const matrix<S,T>&, T);
    template <size_t S, typename T> constexpr matrix<S,T> operator/ (const matrix<S,T>&, T);
    template <size_t S, typename T> constexpr matrix<S,T> operator+ (const matrix<S,T>&, T);
    template <size_t S, typename T> constexpr matrix<S,T> operator- (const matrix<S,T>&, T);

    template <size_t S, typename T> constexpr matrix<S,T> operator* (T, const matrix<S,T>&);
    template <size_t S, typename T> constexpr matrix<S,T> operator/ (T, const matrix<S,T>&);
    template <size_t S, typename T> constexpr matrix<S,T> operator+ (T, const matrix<S,T>&);
    template <size_t S, typename T> constexpr matrix<S,T> operator- (T, const matrix<S,T>&);

    template <size_t S, typename T> constexpr matrix<S,T>& operator*= (matrix<S,T>&, T);
    template <size_t S, typename T> constexpr matrix<S,T>& operator/= (matrix<S,T>&, T);
    template <size_t S, typename T> constexpr matrix<S,T>& operator+= (matrix<S,T>&, T);
    template <size_t S, typename T> constexpr matrix<S,T>& operator-= (matrix<S,T>&, T);


    ///////////////////////////////////////////////////////////////////////////
    template <size_t S, typename T>
    T
    determinant (const matrix<S,T>&);

    template <size_t S, typename T>
    matrix<S,T>
    inverse (const matrix<S,T>&);

    template <size_t S, typename T>
    matrix<S,T>
    transposed (const matrix<S,T>&);

    ///////////////////////////////////////////////////////////////////////////

    template <size_t S, typename T>
    matrix<S,T>
    abs (const matrix<S,T>&);

    template <size_t S, typename T>
    constexpr
    T
    sum (const matrix<S,T>&);


    ///////////////////////////////////////////////////////////////////////////
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


    ///////////////////////////////////////////////////////////////////////////
    template <size_t S, typename T>
    std::ostream& operator<< (std::ostream&, const matrix<S,T>&);
}

#include "matrix.ipp"

#endif
