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
    template <
        std::size_t Rows,
        std::size_t Cols,
        typename T
    >
    struct matrix {
        static constexpr auto rows = Rows;
        static constexpr auto cols = Cols;

        using row_t = util::vector<Cols,T>;


        ///////////////////////////////////////////////////////////////////////
        constexpr matrix () noexcept = default;


        //---------------------------------------------------------------------
        constexpr matrix (const T(&_data)[Rows][Cols]) noexcept:
            values {}
        {
            static_assert (sizeof (*this) == sizeof (T) * Rows * Cols);
            for (std::size_t r = 0; r < Rows; ++r)
                for (std::size_t c = 0; c < Cols; ++c)
                    values[r][c] = _data[r][c];
        }


        //---------------------------------------------------------------------
        template <std::size_t S, typename SelfT>
        constexpr matrix (const util::coord::base<S,T,SelfT> (&_data)[Rows]) noexcept
        {
            for (std::size_t r = 0; r < Rows; ++r)
                for (std::size_t c = 0; c < Cols; ++c)
                    values[r][c] = _data[r][c];
        }


        ///////////////////////////////////////////////////////////////////////
        // index operators return a pointer into the data array so that
        // multidimensional array syntax can be used transparently on this
        // type.
        constexpr row_t& operator[] (std::size_t idx)& { return values[idx]; }
        constexpr const row_t& operator[] (std::size_t idx) const& { return values[idx]; }

        //---------------------------------------------------------------------
        constexpr row_t*
        data (void)& noexcept
        {
            return &values[0];
        }

        //---------------------------------------------------------------------
        constexpr const row_t*
        data (void) const& noexcept
        {
            return &values[0];
        }

        //---------------------------------------------------------------------
        constexpr auto
        begin (void) const& noexcept
        {
            return data ();
        }

        //---------------------------------------------------------------------
        constexpr auto
        begin (void)& noexcept
        {
            return data ();
        }

        //---------------------------------------------------------------------
        constexpr row_t*
        end (void)& noexcept
        {
            return &values[Rows];
        }

        //---------------------------------------------------------------------
        constexpr const row_t*
        end (void) const& noexcept
        {
            return &values[Rows];
        }

        //---------------------------------------------------------------------
        constexpr auto
        cbegin (void) const& noexcept
        {
            return begin ();
        }

        //---------------------------------------------------------------------
        constexpr auto
        cend (void) const& noexcept
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


        template <typename VectorT>
        std::enable_if_t<is_coord_v<VectorT>, VectorT>
        operator* (const VectorT &rhs) const
        {
            VectorT out;

            for (std::size_t r = 0; r < Rows; ++r)
                out[r] = dot (rhs, values[r]);

            return out;
        }

        bool is_affine (void) const;

        template <typename U>
        matrix<Rows,Cols,U>
        cast (void) const noexcept
        {
            matrix out;
            std::copy (cbegin (), cend (), std::begin (out));
            return out;
        }

        // Constant matrices
        static constexpr
        matrix identity (void) noexcept
        {
            auto m = zeroes ();
            for (std::size_t i = 0; i < Rows; ++i)
                m[i][i] = 1;
            return m;
        }

        static constexpr
        matrix zeroes (void) noexcept
        {
            matrix ret {};
            std::fill (std::begin (ret), std::end (ret), row_t{0});
            return ret;
        }


    private:
        row_t values[Rows];
    };


    // Perspective matrices
    template <typename T> matrix<4,4,T> ortho (T left, T right, T bottom, T top, T near, T far);
    template <typename T> matrix<4,4,T> ortho2D (T left, T right, T bottom, T top);
    template <typename T> matrix<4,4,T> perspective (T fov, T aspect, range<T> Z);
    template <typename T> matrix<4,4,T> look_at (point<3,T> eye, point<3,T> target, vector<3,T> up);

    // Affine matrices
    template <typename T> matrix<4,4,T> translation (vector<3,T>);
    template <typename T> matrix<4,4,T> scale       (vector<3,T>);
    template <typename T> matrix<4,4,T> scale       (T);
    template <typename T> matrix<4,4,T> rotation    (T angle, vector<3,T> about);


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
    operator== (const matrix<Rows,Cols,T> &a, const matrix<Rows,Cols,T> &b)
    {
        return std::equal (std::cbegin (a), std::cend (a), std::cbegin (b));
    }


    //-------------------------------------------------------------------------
    template <std::size_t Rows, std::size_t Cols, typename T>
    constexpr
    bool
    operator!= (const matrix<Rows,Cols,T> &a, const matrix<Rows,Cols,T> &b)
    {
        return !(a == b);
    }


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

        // TODO: iterating over r,c rather than c,r will cause an ICE with
        // clang#xxxx: 'X86 DAG->DAG Instruction Selection'.
        //
        // this is likely related to gold and LTO support. for the time being
        // we switch the orders because it appears to confuse the optimiser
        // sufficiently. :(
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


    //-------------------------------------------------------------------------
    template <std::size_t Rows, std::size_t Cols, typename T>
    matrix<Rows,Cols,T>
    inverse (const matrix<Rows,Cols,T>&);


    //-------------------------------------------------------------------------
    template <std::size_t Rows, std::size_t Cols, typename T>
    matrix<Cols,Rows,T>
    transposed (const matrix<Rows,Cols,T>&);


    ///////////////////////////////////////////////////////////////////////////
    template <std::size_t Rows, std::size_t Cols, typename T>
    matrix<Rows,Cols,T>
    abs (const matrix<Rows,Cols,T> &src)
    {
        matrix<Rows,Cols,T> dst;
        for (size_t r = 0; r < Rows; ++r)
            dst[r] = abs (src[r]);
        return dst;
    }


    template <std::size_t Rows, std::size_t Cols, typename T>
    constexpr T
    sum (const matrix<Rows,Cols,T> &src)
    {
        util::vector<Rows,T> accum {};
        for (size_t r = 0; r < Rows; ++r)
            accum[r] = sum (src[r]);
        return sum (accum);
    }


    ///////////////////////////////////////////////////////////////////////////
    #define MATRIX_ELEMENT_OP(OP)                                       \
    template <std::size_t Rows, std::size_t Cols, typename T>           \
    constexpr                                                           \
    matrix<Rows,Cols,T>                                                 \
    operator OP (                                                       \
        const matrix<Rows,Cols,T> &a,                                   \
        const matrix<Rows,Cols,T> &b)                                   \
    {                                                                   \
        matrix<Rows,Cols,T> res {};                                     \
                                                                        \
        for (std::size_t i = 0; i < a.rows; ++i)                        \
            for (std::size_t j = 0; j < a.cols; ++j)                    \
                res[i][j] = a[i][j] OP b[i][j];                         \
                                                                        \
        return res;                                                     \
    }

    MATRIX_ELEMENT_OP(-)
    MATRIX_ELEMENT_OP(+)

    #undef MATRIX_ELEMENT_OP


    ///////////////////////////////////////////////////////////////////////////
    #define MATRIX_SCALAR_OP(OP)                                        \
    template <std::size_t Rows, std::size_t Cols, typename T>           \
    constexpr                                                           \
    matrix<Rows,Cols,T>                                                 \
    operator OP (const matrix<Rows,Cols,T> &m, const T t)               \
    {                                                                   \
        matrix<Rows,Cols,T> res {};                                     \
                                                                        \
        std::transform (                                                \
            std::cbegin (m),                                            \
            std::cend   (m),                                            \
            std::begin  (res),                                          \
            [&t] (auto x) { return x OP t; }                            \
        );                                                              \
                                                                        \
        return res;                                                     \
    }                                                                   \
                                                                        \
                                                                        \
    template <std::size_t Rows, std::size_t Cols, typename T>           \
    constexpr                                                           \
    matrix<Rows,Cols,T>                                                 \
    operator OP (const T t, const matrix<Rows,Cols,T> &m)               \
    {                                                                   \
        return m OP t;                                                  \
    }                                                                   \
                                                                        \
                                                                        \
    template <std::size_t Rows, std::size_t Cols, typename T>           \
    constexpr                                                           \
    matrix<Rows,Cols,T>&                                                \
    operator OP##= (matrix<Rows,Cols,T> &m, T t)                        \
    {                                                                   \
        std::transform (                                                \
            std::cbegin (m),                                            \
            std::cend   (m),                                            \
            std::begin  (m),                                            \
            [&t] (auto x) { return x OP t; }                            \
        );                                                              \
                                                                        \
        return m;                                                       \
    }

    MATRIX_SCALAR_OP(*)
    MATRIX_SCALAR_OP(/)
    MATRIX_SCALAR_OP(+)
    MATRIX_SCALAR_OP(-)

    #undef MATRIX_SCALAR_OP


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

#endif
