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
template <size_t Rows, size_t Cols, typename T>
T*
util::matrix<Rows,Cols,T>::operator[] (size_t idx)
{
    return this->values[idx];
}


//-----------------------------------------------------------------------------
template <size_t Rows, size_t Cols, typename T>
const T*
util::matrix<Rows,Cols,T>::operator[] (size_t idx) const
{
    return this->values[idx];
}


//-----------------------------------------------------------------------------
template <size_t Rows, size_t Cols, typename T>
T*
util::matrix<Rows,Cols,T>::data (void)
{
    return begin ();
}


//-----------------------------------------------------------------------------
template <size_t Rows, size_t Cols, typename T>
const T*
util::matrix<Rows,Cols,T>::data (void) const
{
    return begin ();
}


//-----------------------------------------------------------------------------
template <size_t Rows, size_t Cols, typename T>
const T*
util::matrix<Rows,Cols,T>::begin (void) const
{
    return &(*this)[0][0];
}


//-----------------------------------------------------------------------------
template <size_t Rows, size_t Cols, typename T>
const T*
util::matrix<Rows,Cols,T>::end (void) const
{
    return &(*this)[Rows][0];
}


//-----------------------------------------------------------------------------
template <size_t Rows, size_t Cols, typename T>
const T*
util::matrix<Rows,Cols,T>::cbegin (void) const
{
    return begin ();
}


//-----------------------------------------------------------------------------
template <size_t Rows, size_t Cols, typename T>
const T*
util::matrix<Rows,Cols,T>::cend (void) const
{
    return end ();
}


//-----------------------------------------------------------------------------
template <size_t Rows, size_t Cols, typename T>
T*
util::matrix<Rows,Cols,T>::begin (void)
{
    return &(*this)[0][0];
}


//-----------------------------------------------------------------------------
template <size_t Rows, size_t Cols, typename T>
T*
util::matrix<Rows,Cols,T>::end (void)
{
    return &(*this)[Rows][0];
}


///////////////////////////////////////////////////////////////////////////////
template <size_t Rows, size_t Cols, typename T>
template <typename U>
util::matrix<Rows,Cols,U>
util::matrix<Rows,Cols,T>::cast (void) const
{
    util::matrix<Rows,Cols,T> out;
    std::copy (cbegin (), cend (), std::begin (out));
    return out;
}


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
template <
    std::size_t R1, std::size_t C1,
    std::size_t R2, std::size_t C2,
    typename T
>
constexpr
util::matrix<R1,C2,T>
util::operator* (const matrix<R1,C1,T> &a, const matrix<R2,C2,T> &b)
{
    static_assert (R2 == C1);

    matrix<R1,C2,T> res {0};

    for (size_t r = 0; r < R1; ++r)
        for (size_t c = 0; c < C2; ++c)
            for (size_t i = 0; i < R2; ++i)
                res[r][c] += a[r][i] * b[i][c];

    return res;
}


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
