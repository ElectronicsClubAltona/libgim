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
template <size_t S, typename T>
T
util::determinant (const matrix<S,T> &m)
{
    static_assert (S == 2, "partial specialization for 2 dimensions");

    return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

template float  util::determinant (const matrix<2,float>&);
template double util::determinant (const matrix<2,double>&);


//-----------------------------------------------------------------------------
template <size_t S, typename T>
matrix<S,T>
util::inverse (const matrix<S,T> &m)
{
    static_assert (S == 2, "partial specialization for 2 dimensions");

    return matrix<S,T> {
         m[1][1], -m[0][1],
        -m[1][0],  m[0][0]
    } / determinant (m);
}

template util::matrix<2,float>  util::inverse (const matrix<2,float>&);
template util::matrix<2,double> util::inverse (const matrix<2,double>&);


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
const matrix<S,T>
matrix<S,T>::IDENTITY = { {
    { 1, 0, },
    { 0, 1  }
} };


///////////////////////////////////////////////////////////////////////////////
template struct util::matrix<2,float>;
template struct util::matrix<2,double>;
