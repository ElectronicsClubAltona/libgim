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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#include "matrix.hpp"

using util::matrix;


///////////////////////////////////////////////////////////////////////////////
template <size_t Rows, size_t Cols, typename T>
T
util::determinant (const matrix<Rows,Cols,T> &m)
{
    static_assert (Rows == 2 && Cols == 2);

    return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

template float  util::determinant (const matrix<2,2,float>&);
template double util::determinant (const matrix<2,2,double>&);


///////////////////////////////////////////////////////////////////////////////
template <size_t Rows, size_t Cols, typename T>
matrix<Rows,Cols,T>
util::inverse (const matrix<Rows,Cols,T> &m)
{
    static_assert (Rows == 2 && Cols == 2);

    return matrix<2,2,T> {
         m[1][1], -m[0][1],
        -m[1][0],  m[0][0]
    } / determinant (m);
}


//-----------------------------------------------------------------------------
template util::matrix<2,2,float>  util::inverse (const matrix<2,2,float>&);
template util::matrix<2,2,double> util::inverse (const matrix<2,2,double>&);


///////////////////////////////////////////////////////////////////////////////
template struct util::matrix<2,2,float>;
template struct util::matrix<2,2,double>;
