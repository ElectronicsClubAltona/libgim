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


#ifdef __UTIL_MATRIX_IPP
#error
#endif

#define __UTIL_MATRIX_IPP


///////////////////////////////////////////////////////////////////////////////
#define MATRIX_ELEMENT_OP(OP)                                       \
template <size_t Rows, size_t Cols, typename T>                     \
constexpr                                                           \
util::matrix<Rows,Cols,T>                                           \
util::operator OP (                                                 \
    const util::matrix<Rows,Cols,T> &a,                             \
    const util::matrix<Rows,Cols,T> &b)                             \
{                                                                   \
    util::matrix<Rows,Cols,T> res {};                               \
                                                                    \
    for (size_t i = 0; i < a.rows; ++i)                             \
        for (size_t j = 0; j < a.cols; ++j)                         \
            res[i][j] = a[i][j] OP b[i][j];                         \
                                                                    \
    return res;                                                     \
}

MATRIX_ELEMENT_OP(-)
MATRIX_ELEMENT_OP(+)

#undef MATRIX_ELEMENT_OP


///////////////////////////////////////////////////////////////////////////////
#define MATRIX_SCALAR_OP(OP)                                        \
template <size_t Rows, size_t Cols, typename T>                     \
constexpr                                                           \
util::matrix<Rows,Cols,T>                                           \
util::operator OP (const util::matrix<Rows,Cols,T> &m, const T t)   \
{                                                                   \
    util::matrix<Rows,Cols,T> res {};                               \
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
template <size_t Rows, size_t Cols, typename T>                     \
constexpr                                                           \
util::matrix<Rows,Cols,T>                                           \
util::operator OP (const T t, const util::matrix<Rows,Cols,T> &m)   \
{                                                                   \
    return m OP t;                                                  \
}                                                                   \
                                                                    \
                                                                    \
template <size_t Rows, size_t Cols, typename T>                     \
constexpr                                                           \
util::matrix<Rows,Cols,T>&                                          \
util::operator OP##= (util::matrix<Rows,Cols,T> &m, T t)            \
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


///////////////////////////////////////////////////////////////////////////////
template <size_t Rows, size_t Cols, typename T>
constexpr
util::matrix<Rows,Cols,T>
util::matrix<Rows,Cols,T>::zeroes (void)
{
    return {0};
}


///////////////////////////////////////////////////////////////////////////////
template <size_t Rows, size_t Cols, typename T>
constexpr
util::matrix<Rows,Cols,T>
util::matrix<Rows,Cols,T>::identity (void)
{
    static_assert (Rows == Cols);

    auto m = zeroes ();
    for (size_t i = 0; i < Rows; ++i)
        m[i][i] = 1;
    return m;
}


///////////////////////////////////////////////////////////////////////////////
template <size_t Rows, size_t Cols, typename T>
constexpr
bool
util::operator== (const matrix<Rows,Cols,T> &a, const matrix<Rows,Cols,T> &b)
{
    return std::equal (std::cbegin (a), std::cend (a), std::cbegin (b));
}


//-----------------------------------------------------------------------------
template <size_t Rows, size_t Cols, typename T>
constexpr
bool
util::operator!= (const matrix<Rows,Cols,T> &a, const matrix<Rows,Cols,T> &b)
{
    return !(a == b);
}


///////////////////////////////////////////////////////////////////////////////
template <size_t Rows, size_t Cols, typename T>
util::matrix<Rows,Cols,T>
util::abs (const util::matrix<Rows,Cols,T> &src)
{
    util::matrix<Rows,Cols,T> dst;
    std::transform (std::cbegin (src), std::cend (src), std::begin (dst), util::abs<T>);
    return dst;
}


///////////////////////////////////////////////////////////////////////////////
template <size_t Rows, size_t Cols, typename T>
constexpr
T
util::sum (const util::matrix<Rows, Cols,T> &src)
{
    return sum (std::cbegin (src), std::cend (src));
}
