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
#include <cstdlib>

namespace util {
    template <size_t Rows, size_t Cols, typename T>
    struct matrix {
        static constexpr auto rows = Rows;
        static constexpr auto cols = Cols;

        T values[Rows][Cols];

        ///////////////////////////////////////////////////////////////////////
        // index operators return a pointer into the data array so that
        // multidimensional array syntax can be used transparently on this
        // type.
        constexpr T*
        operator[] (size_t idx) noexcept
        {
            return this->values[idx];
        }

        //---------------------------------------------------------------------
        constexpr const T*
        operator[] (size_t idx) const noexcept
        {
            return this->values[idx];
        }

        //---------------------------------------------------------------------
        constexpr T*
        data (void) noexcept
        {
            return begin ();
        }

        //---------------------------------------------------------------------
        constexpr const T*
        data (void) const noexcept
        {
            return begin ();
        }

        //---------------------------------------------------------------------
        constexpr const T*
        begin (void) const noexcept
        {
            return &(*this)[0][0];
        }

        //---------------------------------------------------------------------
        constexpr const T*
        end (void) const noexcept
        {
            return &(*this)[Rows][0];
        }

        //---------------------------------------------------------------------
        constexpr T*
        begin (void) noexcept
        {
            return &(*this)[0][0];
        }

        //---------------------------------------------------------------------
        constexpr T*
        end (void) noexcept
        {
            return &(*this)[Rows][0];
        }

        //---------------------------------------------------------------------
        constexpr auto
        cbegin (void) const noexcept
        {
            return begin ();
        }

        //---------------------------------------------------------------------
        constexpr auto
        cend (void) const noexcept
        {
            return end ();
        }


        ///////////////////////////////////////////////////////////////////////
        T determinant (void) const;

        matrix inverse (void) const;

        matrix
        inverse_affine (void) const
        {
            // TODO: ensure we have specialisations for typical dimensions
            return inverse ();
        }


        ///////////////////////////////////////////////////////////////////////
        vector<Rows,T> operator* (const vector<Rows,T>&) const;
        point<Rows,T>  operator* (const point<Rows,T> &) const;

        bool is_affine (void) const;

        template <typename U>
        matrix<Rows,Cols,U>
        cast (void) const
        {
            util::matrix<Rows,Cols,T> out;
            std::copy (cbegin (), cend (), std::begin (out));
            return out;
        }

        // Perspective matrices
        static matrix<4,4,T> ortho   (T left, T right, T bottom, T top, T near, T far);
        static matrix<4,4,T> ortho2D (T left, T right, T bottom, T top);
        static matrix<4,4,T> perspective (T fov, T aspect, range<T> Z);
        static matrix<4,4,T> look_at (point<3,T> eye, point<3,T> target, vector<3,T> up);

        // Affine matrices
        static matrix<4,4,T> translation (util::vector<2,T>);
        static matrix<4,4,T> translation (util::vector<3,T>);
        static matrix<4,4,T> scale       (util::vector<3,T>);
        static matrix<4,4,T> scale       (T);
        static matrix<4,4,T> rotation    (T angle, util::vector<3,T> about);

        // Constant matrices
        static constexpr matrix identity ();
        static constexpr matrix zeroes ();
    };


    ///////////////////////////////////////////////////////////////////////////
    // Convert an affine rotation matrix to euler angles.
    //
    // Results are undefined if the matrix is not purely a rotation matrix,
    // or if the dimension is not 3x3 or 4x4.
    template <size_t Rows, size_t Cols, typename T>
    vector<3,T>
    to_euler (const matrix<Rows, Cols, T>&);


    ///////////////////////////////////////////////////////////////////////////
    // logical operations
    template <size_t Rows, size_t Cols, typename T>
    constexpr
    bool
    operator== (const matrix<Rows,Cols,T>&, const matrix<Rows,Cols,T>&);


    template <size_t Rows, size_t Cols, typename T>
    constexpr
    bool
    operator!= (const matrix<Rows,Cols,T>&, const matrix<Rows,Cols,T>&);


    ///////////////////////////////////////////////////////////////////////////
    // element operations
    template <size_t Rows, size_t Cols, typename T>
    constexpr
    matrix<Rows,Cols,T>
    operator+ (const matrix<Rows,Cols,T>&, const matrix<Rows,Cols,T>&);

    template <size_t Rows, size_t Cols, typename T>
    constexpr
    matrix<Rows,Cols,T>
    operator- (const matrix<Rows,Cols,T>&, const matrix<Rows,Cols,T>&);


    ///////////////////////////////////////////////////////////////////////////
    // scalar operations
    template <size_t R, size_t C, typename T> constexpr matrix<R,C,T> operator* (const matrix<R,C,T>&, T);
    template <size_t R, size_t C, typename T> constexpr matrix<R,C,T> operator/ (const matrix<R,C,T>&, T);
    template <size_t R, size_t C, typename T> constexpr matrix<R,C,T> operator+ (const matrix<R,C,T>&, T);
    template <size_t R, size_t C, typename T> constexpr matrix<R,C,T> operator- (const matrix<R,C,T>&, T);

    template <size_t R, size_t C, typename T> constexpr matrix<R,C,T> operator* (T, const matrix<R,C,T>&);
    template <size_t R, size_t C, typename T> constexpr matrix<R,C,T> operator/ (T, const matrix<R,C,T>&);
    template <size_t R, size_t C, typename T> constexpr matrix<R,C,T> operator+ (T, const matrix<R,C,T>&);
    template <size_t R, size_t C, typename T> constexpr matrix<R,C,T> operator- (T, const matrix<R,C,T>&);

    template <size_t R, size_t C, typename T> constexpr matrix<R,C,T>& operator*= (matrix<R,C,T>&, T);
    template <size_t R, size_t C, typename T> constexpr matrix<R,C,T>& operator/= (matrix<R,C,T>&, T);
    template <size_t R, size_t C, typename T> constexpr matrix<R,C,T>& operator+= (matrix<R,C,T>&, T);
    template <size_t R, size_t C, typename T> constexpr matrix<R,C,T>& operator-= (matrix<R,C,T>&, T);


    ///////////////////////////////////////////////////////////////////////////
    // matrix operations
    template <
        size_t R1, size_t C1,
        size_t R2, size_t C2,
        typename T
    >
    constexpr
    matrix<R1,C2,T>
    operator* (const matrix<R1,C1,T> &a, const matrix<R2,C2,T> &b) noexcept
    {
        static_assert (R2 == C1);

        matrix<R1,C2,T> res {0};

        // TODO: iterating over r,c rather than c,r will cause an ICE with
        // clang#xxxx: 'X86 DAG->DAG Instruction Selection'.
        //
        // this is likely related to gold and LTO support. for the time being
        // we switch the orders because it appears to confuse the optimiser
        // sufficiently. :(
        for (size_t c = 0; c < C2; ++c) {
            for (size_t r = 0; r < R1; ++r) {
                T accum{0};

                for (size_t i = 0; i < R2; ++i)
                    accum += a[r][i] * b[i][c];

                res[r][c] = accum;
            }
        }

        return res;
    }


    //-------------------------------------------------------------------------
    template <
        size_t R1, size_t C1,
        size_t R2, size_t C2,
        typename T
    >
    constexpr
    matrix<R1,C2,T>&
    operator*= (matrix<R1,C1,T> &a, const matrix<R2,C2,T> &b)
    { return a = a * b; };


    ///////////////////////////////////////////////////////////////////////////
    template <size_t Rows, size_t Cols, typename T>
    T
    determinant (const matrix<Rows,Cols,T>&);

    template <size_t Rows, size_t Cols, typename T>
    matrix<Rows,Cols,T>
    inverse (const matrix<Rows,Cols,T>&);

    template <size_t Rows, size_t Cols, typename T>
    matrix<Cols,Rows,T>
    transposed (const matrix<Rows,Cols,T>&);

    ///////////////////////////////////////////////////////////////////////////

    template <size_t Rows, size_t Cols, typename T>
    matrix<Rows,Cols,T>
    abs (const matrix<Rows,Cols,T>&);

    template <size_t Rows, size_t Cols, typename T>
    constexpr
    T
    sum (const matrix<Rows,Cols,T>&);


    ///////////////////////////////////////////////////////////////////////////
    template <typename T> using matrix3 = matrix<3,3,T>;
    template <typename T> using matrix4 = matrix<4,4,T>;

    template <size_t Rows, size_t Cols> using matrixf = matrix<Rows,Cols,float>;
    template <size_t Rows, size_t Cols> using matrixd = matrix<Rows,Cols,double>;

    typedef matrix<2,2,float> matrix2f;
    typedef matrix<2,2,double> matrix2d;

    typedef matrix<3,3,float> matrix3f;
    typedef matrix<3,3,double> matrix3d;

    typedef matrix<4,4,float> matrix4f;
    typedef matrix<4,4,double> matrix4d;


    ///////////////////////////////////////////////////////////////////////////
    template <size_t Rows, size_t Cols, typename T>
    std::ostream& operator<< (std::ostream&, const matrix<Rows,Cols,T>&);
}

#include "matrix.ipp"

#endif
