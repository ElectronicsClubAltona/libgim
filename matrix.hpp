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
#include "vector.hpp"

#include <ostream>
#include <cstdlib>

namespace util {
    template <std::size_t Rows, std::size_t Cols, typename T>
    struct matrix {
        static constexpr auto rows = Rows;
        static constexpr auto cols = Cols;

        constexpr matrix () noexcept = default;

        constexpr matrix (const T(&_data)[Rows][Cols]) noexcept:
            values {}
        {
            for (std::size_t r = 0; r < Rows; ++r)
                for (std::size_t c = 0; c < Cols; ++c)
                    values[r][c] = _data[r][c];
        }


        template <std::size_t S, typename SelfT>
        constexpr matrix (const util::coord::base<S,T,SelfT> (&_data)[Rows]) noexcept
        {
            for (std::size_t r = 0; r < Rows; ++r)
                for (std::size_t c = 0; c < Cols; ++c)
                    values[r][c] = _data[r][c];
        }

        T values[rows][cols];

        // index operators return a pointer into the data array so that
        // multidimensional array syntax can be used transparently on this
        // type.
        constexpr auto& operator[] (std::size_t idx) { return values[idx]; }
        constexpr const auto& operator[] (std::size_t idx) const { return values[idx]; }

        T* data (void);
        const T* data (void) const;

        const T* begin (void) const;
        const T* end   (void) const;
        T* begin (void);
        T* end   (void);
        const T* cbegin (void) const;
        const T* cend   (void) const;

        T determinant (void) const;

        matrix  inverse (void) const;
        matrix& invert  (void);
        matrix  inverse_affine (void) const;
        matrix& invert_affine  (void);


        template <typename VectorT>
        std::enable_if_t<is_coord_v<VectorT>, VectorT>
        operator* (const VectorT &rhs) const
        {
            VectorT out;

            for (std::size_t i = 0; i < Rows; ++i)
                out[i] = dot (rhs, values[i]);

            return out;
        }

        bool is_affine (void) const;

        template <typename U>
        matrix<Rows,Cols,U>
        cast (void) const;

        // Constant matrices
        static constexpr matrix identity ();
        static constexpr matrix zeroes ();
    };


    // Perspective matrices
    template <typename T> matrix<4,4,T> ortho (T left, T right, T bottom, T top, T near, T far);
    template <typename T> matrix<4,4,T> ortho2D (T left, T right, T bottom, T top);
    template <typename T> matrix<4,4,T> perspective (T fov, T aspect, range<T> Z);
    template <typename T> matrix<4,4,T> look_at (point<3,T> eye, point<3,T> target, vector<3,T> up);

    // Affine matrices
    template <typename T> matrix<4,4,T> translation (util::vector<3,T>);
    template <typename T> matrix<4,4,T> scale       (util::vector<3,T>);
    template <typename T> matrix<4,4,T> scale       (T);
    template <typename T> matrix<4,4,T> rotation    (T angle, util::vector<3,T> about);


    ///////////////////////////////////////////////////////////////////////////
    // Convert an affine rotation matrix to euler angles.
    //
    // Results are undefined if the matrix is not purely a rotation matrix,
    // or if the dimension is not 3x3 or 4x4.
    template <std::size_t Rows, std::size_t Cols, typename T>
    vector<3,T>
    to_euler (const matrix<Rows, Cols, T>&);


    ///////////////////////////////////////////////////////////////////////////
    // logical operations
    template <std::size_t Rows, std::size_t Cols, typename T>
    constexpr
    bool
    operator== (const matrix<Rows,Cols,T>&, const matrix<Rows,Cols,T>&);


    template <std::size_t Rows, std::size_t Cols, typename T>
    constexpr
    bool
    operator!= (const matrix<Rows,Cols,T>&, const matrix<Rows,Cols,T>&);


    ///////////////////////////////////////////////////////////////////////////
    // element operations
    template <std::size_t Rows, std::size_t Cols, typename T>
    constexpr
    matrix<Rows,Cols,T>
    operator+ (const matrix<Rows,Cols,T>&, const matrix<Rows,Cols,T>&);

    template <std::size_t Rows, std::size_t Cols, typename T>
    constexpr
    matrix<Rows,Cols,T>
    operator- (const matrix<Rows,Cols,T>&, const matrix<Rows,Cols,T>&);


    template <std::size_t Rows, std::size_t Cols, typename T>
    constexpr
    matrix<Rows,Cols,T>
    div (const matrix<Rows,Cols,T> &a, const matrix<Rows,Cols,T> &b)
    {
        matrix<Rows,Cols,T> out {};
        for (std::size_t r = 0; r < Rows; ++r)
            for (std::size_t c = 0; c < Cols; ++c)
                out[r][c] = a[r][c] / b[r][c];
        return out;
    }


    template <std::size_t Rows, std::size_t Cols, typename T>
    constexpr T
    max (const matrix<Rows,Cols,T> &m)
    {
        T val = m[0][0];
        for (std::size_t r = 0; r < Rows; ++r)
            for (std::size_t c = 0; c < Cols; ++c)
                val = max (val, m[r][c]);
        return val;
    }


    ///////////////////////////////////////////////////////////////////////////
    // scalar operations
    template <std::size_t R, std::size_t C, typename T> constexpr matrix<R,C,T> operator* (const matrix<R,C,T>&, T);
    template <std::size_t R, std::size_t C, typename T> constexpr matrix<R,C,T> operator/ (const matrix<R,C,T>&, T);
    template <std::size_t R, std::size_t C, typename T> constexpr matrix<R,C,T> operator+ (const matrix<R,C,T>&, T);
    template <std::size_t R, std::size_t C, typename T> constexpr matrix<R,C,T> operator- (const matrix<R,C,T>&, T);

    template <std::size_t R, std::size_t C, typename T> constexpr matrix<R,C,T> operator* (T, const matrix<R,C,T>&);
    template <std::size_t R, std::size_t C, typename T> constexpr matrix<R,C,T> operator/ (T, const matrix<R,C,T>&);
    template <std::size_t R, std::size_t C, typename T> constexpr matrix<R,C,T> operator+ (T, const matrix<R,C,T>&);
    template <std::size_t R, std::size_t C, typename T> constexpr matrix<R,C,T> operator- (T, const matrix<R,C,T>&);

    template <std::size_t R, std::size_t C, typename T> constexpr matrix<R,C,T>& operator*= (matrix<R,C,T>&, T);
    template <std::size_t R, std::size_t C, typename T> constexpr matrix<R,C,T>& operator/= (matrix<R,C,T>&, T);
    template <std::size_t R, std::size_t C, typename T> constexpr matrix<R,C,T>& operator+= (matrix<R,C,T>&, T);
    template <std::size_t R, std::size_t C, typename T> constexpr matrix<R,C,T>& operator-= (matrix<R,C,T>&, T);


    ///////////////////////////////////////////////////////////////////////////
    // matrix operations
    template <
        std::size_t R1, //int C1,
        std::size_t RC, //int R2,
        std::size_t C2,
        typename T
    >
    constexpr
    matrix<R1,C2,T>
    operator* (const matrix<R1,RC,T>&a, const matrix<RC,C2,T>&b)
    {
        matrix<R1,C2,T> res {};

        // we use a column first iteration approach because otherwise we risk
        // triggering clang#????
        for (std::size_t c = 0; c < C2; ++c) {
            for (std::size_t r = 0; r < R1; ++r) {
                T accum {0};

                for (std::size_t i = 0; i < RC; ++i)
                    accum += a[r][i] * b[i][c];

                res[r][c] = accum;
            }
        }

        return res;
    }


    //-------------------------------------------------------------------------
    template <
        std::size_t R1, std::size_t C1,
        std::size_t R2, std::size_t C2,
        typename T
    >
    constexpr
    matrix<R1,C2,T>&
    operator*= (matrix<R1,C1,T> &a, const matrix<R2,C2,T> &b)
    { return a = a * b; };


    ///////////////////////////////////////////////////////////////////////////
    template <std::size_t Rows, std::size_t Cols, typename T>
    T
    determinant (const matrix<Rows,Cols,T>&);

    template <std::size_t Rows, std::size_t Cols, typename T>
    matrix<Rows,Cols,T>
    inverse (const matrix<Rows,Cols,T>&);

    template <std::size_t Rows, std::size_t Cols, typename T>
    matrix<Cols,Rows,T>
    transposed (const matrix<Rows,Cols,T>&);

    ///////////////////////////////////////////////////////////////////////////

    template <std::size_t Rows, std::size_t Cols, typename T>
    matrix<Rows,Cols,T>
    abs (const matrix<Rows,Cols,T>&);

    template <std::size_t Rows, std::size_t Cols, typename T>
    constexpr
    T
    sum (const matrix<Rows,Cols,T>&);


    ///////////////////////////////////////////////////////////////////////////
    template <typename T> using matrix3 = matrix<3,3,T>;
    template <typename T> using matrix4 = matrix<4,4,T>;

    template <std::size_t Rows, std::size_t Cols> using matrixf = matrix<Rows,Cols,float>;
    template <std::size_t Rows, std::size_t Cols> using matrixd = matrix<Rows,Cols,double>;

    typedef matrix<2,2,float> matrix2f;
    typedef matrix<2,2,double> matrix2d;

    typedef matrix<3,3,float> matrix3f;
    typedef matrix<3,3,double> matrix3d;

    typedef matrix<4,4,float> matrix4f;
    typedef matrix<4,4,double> matrix4d;


    ///////////////////////////////////////////////////////////////////////////
    template <std::size_t Rows, std::size_t Cols, typename T>
    std::ostream& operator<< (std::ostream&, const matrix<Rows,Cols,T>&);
}

#include "matrix.ipp"

#endif
