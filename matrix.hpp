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
 * Copyright 2010 Danny Robson <danny@blubinc.net>
 */

#ifndef __UTIL_MATRIX_HPP
#define __UTIL_MATRIX_HPP

#include "vector.hpp"

#include <assert.h>
#include <algorithm>
#include <stdexcept>
#include <initializer_list>
#include <iostream>

namespace maths {
    class matrix {
        protected:
            size_t  m_rows,
                    m_columns;
            double *restrict m_data;

        public:
            matrix (size_t _rows, size_t _columns);

            matrix (size_t                                _rows,
                    size_t                                _columns,
                    const std::initializer_list <double> &_data);
            matrix (const std::initializer_list <vector> &_data);

            matrix (const matrix &rhs);
            matrix (matrix &&rhs);

            ~matrix();

            void sanity (void) const;

            const double * operator [] (unsigned int row) const;
            double *       operator [] (unsigned int row);

            const double * data (void) const;
            
            matrix& operator  =(const matrix &rhs);
            matrix  operator * (double        scalar) const;
            matrix& operator *=(double        scalar);
            matrix  operator * (const matrix &rhs)    const;
            matrix& operator *=(const matrix &rhs);
            matrix& operator /=(double        scalar);
            matrix  operator + (double        scalar) const;
            matrix& operator +=(double        scalar);
            matrix& operator -=(double        scalar);
            bool    operator ==(const matrix &rhs)    const;

            //matrix  transpose (void) const { ; }
            
            size_t rows    (void) const;
            size_t columns (void) const;
            size_t size    (void) const;

            /// Checks if this is a sqaure matrix, with a zero final column
            /// and row (excepting the final diagonal entry).
            bool is_homogeneous (void) const;
            bool is_square      (void) const;
            bool is_magic       (void) const;
        public:
            double determinant (void) const;
            matrix inverse     (void) const;

        protected:
            double determinant2x2 (void) const;
            double determinant3x3 (void) const;
            double determinant4x4 (void) const;

            matrix inverse2x2 (void) const;
            matrix inverse3x3 (void) const;
            matrix inverse4x4 (void) const;

        public:
            static matrix zeroes   (size_t n);
            static matrix zeroes   (size_t rows, size_t columns);
            static matrix identity (size_t n);

            /// Generate a magic square of order 'n'
            static matrix magic    (size_t n);

        protected:
            /// Generate a magic square with 'n' odd
            static matrix magic_odd         (size_t n);
            /// Generate a magic square with 'n' divisible by 2, and not 4
            static matrix magic_even_single (size_t n);
            /// Generate a magic square with 'n' divisible by 4, and not 2
            static matrix magic_even_double (size_t n);
    };

    class not_invertible : public std::runtime_error {
        public:
            not_invertible (): 
                std::runtime_error ("not_invertible")
            { ; }
    };
}


#endif
