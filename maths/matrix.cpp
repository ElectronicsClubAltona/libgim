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
 * Copyright 2010 Danny Robson <danny@nerdcruft.net>
 */

#include "matrix.hpp"

#include "debug.hpp"
#include "range.hpp"
#include "maths.hpp"

#include <algorithm>

using namespace util;
using namespace maths;

matrix::matrix (size_t _rows, size_t _columns):
    m_rows    (_rows),
    m_columns (_columns),
    m_data    (new double[_rows * _columns])
{
}


matrix::matrix (size_t                                _rows,
                size_t                                _columns,
                const std::initializer_list <double> &_data):
    m_rows    (_rows),
    m_columns (_columns)
{
    if (size () != _data.size ())
        throw std::runtime_error ("element and initializer size differs");
    CHECK_HARD (m_rows * m_columns == _data.size());

    m_data.reset (new double[size ()]);
    std::copy (_data.begin (), _data.end (), m_data.get ());
}


matrix::matrix (const std::initializer_list <vector> &rhs):
    m_rows    (rhs.size    ()),
    m_columns (rhs.begin()->size ()),
    m_data    (new double[m_rows * m_columns]) 
{
    double *row_cursor = m_data.get ();

    for (auto i = rhs.begin (); i != rhs.end (); ++i) {
        CHECK (i->size () == m_columns);

        std::copy (i->data (), i->data () + i->size (), row_cursor);
        row_cursor += m_columns;
    }
}


matrix::matrix (const matrix &rhs):
    m_rows    (rhs.m_rows),
    m_columns (rhs.m_columns)
{
    m_data.reset (new double [m_rows * m_columns]);
    std::copy (rhs.m_data.get (), rhs.m_data.get () + m_rows * m_columns, m_data.get ());
}


matrix::matrix (matrix &&rhs):
    m_rows    (rhs.m_rows),
    m_columns (rhs.m_columns),
    m_data    (std::move (rhs.m_data))
{
}


matrix::~matrix()
{ ; }


void
matrix::sanity (void) const {
    CHECK (m_rows    > 0);
    CHECK (m_columns > 0);
    CHECK (m_data != nullptr);
}


const double *
matrix::operator [] (unsigned int row) const {
    CHECK_HARD (row < m_rows);
    return m_data.get () + row * m_columns;
}


double *
matrix::operator [] (unsigned int row) {
    CHECK_HARD (row < m_rows);
    return m_data.get () + row * m_columns;
}


const double *
matrix::data (void) const
    { return m_data.get (); }


matrix&
matrix::operator =(const matrix& rhs) {
    if (size () != rhs.size ()) {
        m_data.reset (new double [rhs.rows () * rhs.columns ()]);
    }

    m_rows    = rhs.m_rows;
    m_columns = rhs.m_columns;
    std::copy (rhs.m_data.get (), rhs.m_data.get () + m_rows * m_columns, m_data.get ());

    return *this;
}


matrix
matrix::operator * (double scalar) const {
    matrix val (*this);

    for (unsigned int i = 0; i < m_rows; ++i)
        for (unsigned int j = 0; j < m_columns; ++j)
            val[i][j] *= scalar;

    return val;
}


matrix&
matrix::operator *=(double scalar) {
    for (unsigned int i = 0; i < m_rows; ++i)
        for (unsigned int j = 0; j < m_columns; ++j)
            (*this)[i][j] *= scalar;

    return *this;
}


matrix&
matrix::operator /= (double scalar)
    { return (*this) *= (1.0 / scalar); }


matrix
matrix::operator  + (double scalar) const {
    matrix val (*this);
    for (unsigned int i = 0; i < m_rows; ++i)
        for (unsigned int j = 0; j < m_columns; ++j)
            val[i][j] += scalar;

    return val;
}


matrix&
matrix::operator +=(double scalar) {
    for (unsigned int i = 0; i < m_rows; ++i)
        for (unsigned int j = 0; j < m_columns; ++j)
            (*this)[i][j] += scalar;

    return *this;
}


matrix
matrix::operator * (const matrix& rhs) const {
    if (m_columns != rhs.rows ())
        throw std::invalid_argument ("matrices size mismatch in multiplication");

    matrix val (matrix::zeroes (m_rows, rhs.columns ()));

    for (unsigned int i = 0; i < m_rows; ++i) 
        for (unsigned int j = 0; j < rhs.columns (); ++j)
            for (unsigned int k = 0; k < m_columns; ++k)
                val[i][j] += (*this)[i][k] * rhs[k][j];

    return val;
}


matrix&
matrix::operator *=(const matrix& rhs)
    { return *this = *this * rhs; }


bool
matrix::operator ==(const matrix& rhs) const {
    if (rhs.rows    () != rows     () ||
        rhs.columns () != columns  ())
        return false;

    return std::equal (m_data.get (), m_data.get () + size (), rhs.data ());
}


//matrix  transpose (void) const { ; }


size_t
matrix::rows (void) const 
    { return m_rows; }


size_t
matrix::columns (void) const
    { return m_columns; }


size_t
matrix::size (void) const 
    { return rows () * columns (); }


bool
matrix::is_square (void) const 
    { return m_rows == m_columns; }


bool
matrix::is_magic (void) const {
    if (!is_square ())
        return false;

    unsigned int expected = m_rows * (m_rows * m_rows + 1) / 2;
    range<double> numbers (1, m_rows * m_rows);

    for (unsigned int i = 0; i < m_rows; ++i) {
        unsigned int sum1 = 0, sum2 = 0;

        for (unsigned int j = 0; j < m_columns; ++j) {
            double a = (*this)[i][j],
                   b = (*this)[j][i];

            if (!exactly_zero (a) || !exactly_zero (b))
                return false;

            if (!numbers.contains (a) ||
                !numbers.contains (b))
                return false;

            sum1 += static_cast<unsigned> (a);
            sum2 += static_cast<unsigned> (b);
        }
        
        if (sum1 != expected || sum2 != expected)
            return false;
    }

    return true;
}


bool
matrix::is_homogeneous (void) const {
    if (m_rows != m_columns)
        return false;

    // Check the final row is all zeroes
    for (unsigned int i = 0; i < m_columns - 1; ++i) {
        if (!almost_equal ((*this)[m_rows - 1][i], 0.))
            return false;
    }

    // Except for the last element, which has to be one
    return almost_equal ((*this)[m_rows - 1][m_columns - 1], 1.);
}


double
matrix::determinant (void) const {
    if (m_rows != m_columns)
        not_implemented ();

    switch (m_rows) {
        case 2: return determinant2x2 ();
        case 3: return determinant3x3 ();
        case 4: return determinant4x4 ();
    }

    not_implemented ();
}


// With matrix A = [ a, b ]
//                 [ c, d ]
//  
// det (A) = ad - bc

double
matrix::determinant2x2 (void) const {
    CHECK_EQ (m_rows,    2);
    CHECK_EQ (m_columns, 2);

    return (*this)[0][0] * (*this)[1][1] -
           (*this)[0][1] * (*this)[1][0];
}



//                   [ a, b, c ]
// Given matrix A =  [ d, e, f ]
//                   [ g, h, i ]
//
//  det (A) = aei + bfg + cdh - afg - bdi - ceg
//  det (A) = a(ei - fg) + b(fg - di) + c(dh - eg)
double
matrix::determinant3x3 (void) const {
    CHECK_EQ (m_rows,    3);
    CHECK_EQ (m_columns, 3);

    return (*this)[0][0] * (*this)[1][1] * (*this)[2][2] + // aei
           (*this)[0][1] * (*this)[1][2] * (*this)[2][0] + // bfg
           (*this)[0][2] * (*this)[1][0] * (*this)[2][1] - // cdh
           (*this)[0][0] * (*this)[1][2] * (*this)[2][1] - // afh
           (*this)[0][1] * (*this)[1][0] * (*this)[2][2] - // bdi
           (*this)[0][2] * (*this)[1][1] * (*this)[2][0];  // ceg
}


// From libMathematics, http://www.geometrictools.com/ 
double 
matrix::determinant4x4 (void) const {
    CHECK_EQ (m_rows,    4);
    CHECK_EQ (m_columns, 4);

    double a0 = m_data[ 0] * m_data[ 5] - m_data[ 1] * m_data[ 4],
           a1 = m_data[ 0] * m_data[ 6] - m_data[ 2] * m_data[ 4],
           a2 = m_data[ 0] * m_data[ 7] - m_data[ 3] * m_data[ 4],
           a3 = m_data[ 1] * m_data[ 6] - m_data[ 2] * m_data[ 5],
           a4 = m_data[ 1] * m_data[ 7] - m_data[ 3] * m_data[ 5],
           a5 = m_data[ 2] * m_data[ 7] - m_data[ 3] * m_data[ 6],
           b0 = m_data[ 8] * m_data[13] - m_data[ 9] * m_data[12],
           b1 = m_data[ 8] * m_data[14] - m_data[10] * m_data[12],
           b2 = m_data[ 8] * m_data[15] - m_data[11] * m_data[12],
           b3 = m_data[ 9] * m_data[14] - m_data[10] * m_data[13],
           b4 = m_data[ 9] * m_data[15] - m_data[11] * m_data[13],
           b5 = m_data[10] * m_data[15] - m_data[11] * m_data[14];

    return a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
}


matrix
matrix::inverse (void) const {
    if (m_rows != m_columns)
        not_implemented ();

    switch (m_rows) {
        case  2: return inverse2x2 ();
        case  3: return inverse3x3 ();
        case  4: return inverse4x4 ();
    }

    not_implemented ();
}


matrix
matrix::inverse2x2 (void) const {
    CHECK (m_rows    == 2);
    CHECK (m_columns == 2);

    double det = determinant2x2 ();
    if (almost_equal (det, 0.))
        throw not_invertible ();
    return matrix (2, 2, {  (*this)[1][1], -(*this)[0][1],
                           -(*this)[1][0],  (*this)[0][0] }) /= det;
}


//                   [ a, b, c ]
// Given matrix A =  [ d, e, f ]
//                   [ g, h, i ]
//
matrix
matrix::inverse3x3 (void) const {
    CHECK (m_rows    == 3);
    CHECK (m_columns == 3);

    double det = determinant3x3();
    if (almost_equal (det, 0.))
        throw not_invertible ();

    matrix val (m_rows, m_columns, {
        (*this)[1][1] * (*this)[2][2] - (*this)[1][2] * (*this)[2][1], // ei - fh
        (*this)[0][2] * (*this)[2][1] - (*this)[0][1] * (*this)[2][2], // ch - bi
        (*this)[0][1] * (*this)[1][2] - (*this)[0][2] * (*this)[1][1], // bf - ce
        (*this)[1][2] * (*this)[2][0] - (*this)[1][0] * (*this)[2][2], // fg - di
        (*this)[0][0] * (*this)[2][2] - (*this)[0][2] * (*this)[2][0], // ai - cg
        (*this)[0][2] * (*this)[1][0] - (*this)[0][0] * (*this)[1][2], // cd - af
        (*this)[1][0] * (*this)[2][1] - (*this)[1][1] * (*this)[2][0], // dh - eg
        (*this)[0][1] * (*this)[2][0] - (*this)[0][0] * (*this)[2][1], // bg - ah
        (*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0]  // ae - bd
    });

    return val /= det;

    //matrix val ({ vector::cross ((*this)[1], (*this)[2], 3),
    //              vector::cross ((*this)[2], (*this)[0], 3),
    //              vector::cross ((*this)[0], (*this)[1], 3) });
    //return val /= determinant3x3 ();
}


matrix
matrix::inverse4x4 (void) const {
    double a0 = m_data[ 0] * m_data[ 5] - m_data[ 1] * m_data[ 4],
           a1 = m_data[ 0] * m_data[ 6] - m_data[ 2] * m_data[ 4],
           a2 = m_data[ 0] * m_data[ 7] - m_data[ 3] * m_data[ 4],
           a3 = m_data[ 1] * m_data[ 6] - m_data[ 2] * m_data[ 5],
           a4 = m_data[ 1] * m_data[ 7] - m_data[ 3] * m_data[ 5],
           a5 = m_data[ 2] * m_data[ 7] - m_data[ 3] * m_data[ 6],
           b0 = m_data[ 8] * m_data[13] - m_data[ 9] * m_data[12],
           b1 = m_data[ 8] * m_data[14] - m_data[10] * m_data[12],
           b2 = m_data[ 8] * m_data[15] - m_data[11] * m_data[12],
           b3 = m_data[ 9] * m_data[14] - m_data[10] * m_data[13],
           b4 = m_data[ 9] * m_data[15] - m_data[11] * m_data[13],
           b5 = m_data[10] * m_data[15] - m_data[11] * m_data[14];

    double det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
    if (almost_equal (det, 0.))
        throw not_invertible ();

    return matrix (4, 4, {
        + m_data[ 5] * b5 - m_data[ 6] * b4 + m_data[ 7] * b3,
        - m_data[ 1] * b5 + m_data[ 2] * b4 - m_data[ 3] * b3,
        + m_data[13] * a5 - m_data[14] * a4 + m_data[15] * a3,
        - m_data[ 9] * a5 + m_data[10] * a4 - m_data[11] * a3,
        - m_data[ 4] * b5 + m_data[ 6] * b2 - m_data[ 7] * b1,
        + m_data[ 0] * b5 - m_data[ 2] * b2 + m_data[ 3] * b1,
        - m_data[12] * a5 + m_data[14] * a2 - m_data[15] * a1,
        + m_data[ 8] * a5 - m_data[10] * a2 + m_data[11] * a1,
        + m_data[ 4] * b4 - m_data[ 5] * b2 + m_data[ 7] * b0,
        - m_data[ 0] * b4 + m_data[ 1] * b2 - m_data[ 3] * b0,
        + m_data[12] * a4 - m_data[13] * a2 + m_data[15] * a0,
        - m_data[ 8] * a4 + m_data[ 9] * a2 - m_data[11] * a0,
        - m_data[ 4] * b3 + m_data[ 5] * b1 - m_data[ 6] * b0,
        + m_data[ 0] * b3 - m_data[ 1] * b1 + m_data[ 2] * b0,
        - m_data[12] * a3 + m_data[13] * a1 - m_data[14] * a0,
        + m_data[ 8] * a3 - m_data[ 9] * a1 + m_data[10] * a0
    }) /= det;
}



matrix
matrix::zeroes (size_t diag)
        { return zeroes (diag, diag); }

        
matrix
matrix::zeroes (size_t rows, size_t columns) {
    matrix m (rows, columns);
    std::fill (m.m_data.get (), m.m_data.get () + m.size (), 0.0);
    return m;
}


matrix
matrix::identity (size_t diag) {
    matrix val (zeroes (diag));
    for (unsigned int i = 0; i < diag; ++i)
        val[i][i] = 1.0;
    return val;
}


matrix
matrix::magic (size_t n) {
    CHECK_HARD (n > 2);

    if (n % 2 == 1)
        return magic_odd (n);

    if (n % 4 == 0)
        return magic_even_single (n);

    return magic_even_double (n);
}


// Use the 'siamese' method. Start from the top centre, progress up-left one.
// If filled then drop down one row instead. Wrap around indexing.
matrix
matrix::magic_odd (size_t n) {
    CHECK_HARD (n > 2);
    CHECK_HARD (n % 2 == 1);

    matrix val (zeroes (n));
    for (unsigned int i = 1, x = n / 2, y = 0; i <= n * n; ++i) {
        val[y][x] = i;

        unsigned int x1 = (x +     1) % n,
                     y1 = (y + n - 1) % n;

        if (!almost_equal (val[y1][x1], 0)) {
            x1 = x;
            y1 = (y + 1) % n;
        }

        x = x1;
        y = y1;
    }

    return val;
}


matrix
matrix::magic_even_single (size_t)
    { not_implemented (); }


matrix
matrix::magic_even_double (size_t)
    { not_implemented (); }

