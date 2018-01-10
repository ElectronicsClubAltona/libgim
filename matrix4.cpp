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


//-----------------------------------------------------------------------------
template <std::size_t Rows, std::size_t Cols, typename T>
T
util::determinant (const matrix<Rows,Cols,T> &m)
{
    static_assert (Rows == 4 && Cols == 4);

    return m[0][3] * m[1][2] * m[2][1] * m[3][0] - m[0][2] * m[1][3] * m[2][1] * m[3][0] -
           m[0][3] * m[1][1] * m[2][2] * m[3][0] + m[0][1] * m[1][3] * m[2][2] * m[3][0] +
           m[0][2] * m[1][1] * m[2][3] * m[3][0] - m[0][1] * m[1][2] * m[2][3] * m[3][0] -
           m[0][3] * m[1][2] * m[2][0] * m[3][1] + m[0][2] * m[1][3] * m[2][0] * m[3][1] +
           m[0][3] * m[1][0] * m[2][2] * m[3][1] - m[0][0] * m[1][3] * m[2][2] * m[3][1] -
           m[0][2] * m[1][0] * m[2][3] * m[3][1] + m[0][0] * m[1][2] * m[2][3] * m[3][1] +
           m[0][3] * m[1][1] * m[2][0] * m[3][2] - m[0][1] * m[1][3] * m[2][0] * m[3][2] -
           m[0][3] * m[1][0] * m[2][1] * m[3][2] + m[0][0] * m[1][3] * m[2][1] * m[3][2] +
           m[0][1] * m[1][0] * m[2][3] * m[3][2] - m[0][0] * m[1][1] * m[2][3] * m[3][2] -
           m[0][2] * m[1][1] * m[2][0] * m[3][3] + m[0][1] * m[1][2] * m[2][0] * m[3][3] +
           m[0][2] * m[1][0] * m[2][1] * m[3][3] - m[0][0] * m[1][2] * m[2][1] * m[3][3] -
           m[0][1] * m[1][0] * m[2][2] * m[3][3] + m[0][0] * m[1][1] * m[2][2] * m[3][3];
}

template float  util::determinant (const matrix<4,4,float>&);
template double util::determinant (const matrix<4,4,double>&);


//-----------------------------------------------------------------------------
template <std::size_t Rows, std::size_t Cols, typename T>
matrix<Rows,Cols,T>
util::inverse (const matrix<Rows,Cols,T> &m)
{
    static_assert (Rows == 4 && Cols == 4);

    return matrix<4,4,T> {{
        {  m[1][2]*m[2][3]*m[3][1]
          -m[1][3]*m[2][2]*m[3][1]
          +m[1][3]*m[2][1]*m[3][2]
          -m[1][1]*m[2][3]*m[3][2]
          -m[1][2]*m[2][1]*m[3][3]
          +m[1][1]*m[2][2]*m[3][3],

           m[0][3]*m[2][2]*m[3][1]
          -m[0][2]*m[2][3]*m[3][1]
          -m[0][3]*m[2][1]*m[3][2]
          +m[0][1]*m[2][3]*m[3][2]
          +m[0][2]*m[2][1]*m[3][3]
          -m[0][1]*m[2][2]*m[3][3],

           m[0][2]*m[1][3]*m[3][1]
          -m[0][3]*m[1][2]*m[3][1]
          +m[0][3]*m[1][1]*m[3][2]
          -m[0][1]*m[1][3]*m[3][2]
          -m[0][2]*m[1][1]*m[3][3]
          +m[0][1]*m[1][2]*m[3][3],

           m[0][3]*m[1][2]*m[2][1]
          -m[0][2]*m[1][3]*m[2][1]
          -m[0][3]*m[1][1]*m[2][2]
          +m[0][1]*m[1][3]*m[2][2]
          +m[0][2]*m[1][1]*m[2][3]
          -m[0][1]*m[1][2]*m[2][3], },

        {  m[1][3]*m[2][2]*m[3][0]
          -m[1][2]*m[2][3]*m[3][0]
          -m[1][3]*m[2][0]*m[3][2]
          +m[1][0]*m[2][3]*m[3][2]
          +m[1][2]*m[2][0]*m[3][3]
          -m[1][0]*m[2][2]*m[3][3],

           m[0][2]*m[2][3]*m[3][0]
          -m[0][3]*m[2][2]*m[3][0]
          +m[0][3]*m[2][0]*m[3][2]
          -m[0][0]*m[2][3]*m[3][2]
          -m[0][2]*m[2][0]*m[3][3]
          +m[0][0]*m[2][2]*m[3][3],

           m[0][3]*m[1][2]*m[3][0]
          -m[0][2]*m[1][3]*m[3][0]
          -m[0][3]*m[1][0]*m[3][2]
          +m[0][0]*m[1][3]*m[3][2]
          +m[0][2]*m[1][0]*m[3][3]
          -m[0][0]*m[1][2]*m[3][3],

           m[0][2]*m[1][3]*m[2][0]
          -m[0][3]*m[1][2]*m[2][0]
          +m[0][3]*m[1][0]*m[2][2]
          -m[0][0]*m[1][3]*m[2][2]
          -m[0][2]*m[1][0]*m[2][3]
          +m[0][0]*m[1][2]*m[2][3], },

        {  m[1][1]*m[2][3]*m[3][0]
          -m[1][3]*m[2][1]*m[3][0]
          +m[1][3]*m[2][0]*m[3][1]
          -m[1][0]*m[2][3]*m[3][1]
          -m[1][1]*m[2][0]*m[3][3]
          +m[1][0]*m[2][1]*m[3][3],

           m[0][3]*m[2][1]*m[3][0]
          -m[0][1]*m[2][3]*m[3][0]
          -m[0][3]*m[2][0]*m[3][1]
          +m[0][0]*m[2][3]*m[3][1]
          +m[0][1]*m[2][0]*m[3][3]
          -m[0][0]*m[2][1]*m[3][3],

           m[0][1]*m[1][3]*m[3][0]
          -m[0][3]*m[1][1]*m[3][0]
          +m[0][3]*m[1][0]*m[3][1]
          -m[0][0]*m[1][3]*m[3][1]
          -m[0][1]*m[1][0]*m[3][3]
          +m[0][0]*m[1][1]*m[3][3],

           m[0][3]*m[1][1]*m[2][0]
          -m[0][1]*m[1][3]*m[2][0]
          -m[0][3]*m[1][0]*m[2][1]
          +m[0][0]*m[1][3]*m[2][1]
          +m[0][1]*m[1][0]*m[2][3]
          -m[0][0]*m[1][1]*m[2][3], },

        {  m[1][2]*m[2][1]*m[3][0]
          -m[1][1]*m[2][2]*m[3][0]
          -m[1][2]*m[2][0]*m[3][1]
          +m[1][0]*m[2][2]*m[3][1]
          +m[1][1]*m[2][0]*m[3][2]
          -m[1][0]*m[2][1]*m[3][2],

           m[0][1]*m[2][2]*m[3][0]
          -m[0][2]*m[2][1]*m[3][0]
          +m[0][2]*m[2][0]*m[3][1]
          -m[0][0]*m[2][2]*m[3][1]
          -m[0][1]*m[2][0]*m[3][2]
          +m[0][0]*m[2][1]*m[3][2],

           m[0][2]*m[1][1]*m[3][0]
          -m[0][1]*m[1][2]*m[3][0]
          -m[0][2]*m[1][0]*m[3][1]
          +m[0][0]*m[1][2]*m[3][1]
          +m[0][1]*m[1][0]*m[3][2]
          -m[0][0]*m[1][1]*m[3][2],

           m[0][1]*m[1][2]*m[2][0]
          -m[0][2]*m[1][1]*m[2][0]
          +m[0][2]*m[1][0]*m[2][1]
          -m[0][0]*m[1][2]*m[2][1]
          -m[0][1]*m[1][0]*m[2][2]
          +m[0][0]*m[1][1]*m[2][2], }
    }} / determinant (m);
}

template util::matrix<4,4,float>  util::inverse (const matrix<4,4,float>&);
template util::matrix<4,4,double> util::inverse (const matrix<4,4,double>&);


///////////////////////////////////////////////////////////////////////////////
template struct util::matrix<4,4,float>;
template struct util::matrix<4,4,double>;
