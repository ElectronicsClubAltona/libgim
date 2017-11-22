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
 * Copyright 2010-2016 Danny Robson <danny@nerdcruft.net>
 */

#include "extent.hpp"

#include "debug.hpp"
#include "maths.hpp"

#include <algorithm>
#include <numeric>

using util::extent;
using util::extent_range;


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
extent<S,T>::extent (vector<S,T> _v)
{
    std::transform (std::begin (_v),
                    std::end   (_v),
                    this->begin (),
                    [] (auto i) {
        // using std::abs gives unsigned abs warnings under clang. this tricks
        // it sufficiently to quiet the warnings.
        return i < 0 ? -i : i;
    });
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
extent<S,T>
extent<S,T>::expanded (util::vector<S,T> mag) const
{
    return *this + mag;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
extent<S,T>
extent<S,T>::expanded (T t) const
{
    return *this + util::vector<S,T> {t};
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
extent<S,T>
extent<S,T>::contracted (vector<S,T> t) const
{
    return *this - t;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
extent<S,T>
extent<S,T>::contracted (T t) const
{
    return *this - vector<S,T> {t};
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
bool
extent<S,T>::empty (void) const
{
    return almost_zero (area());
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
extent_range<S,T>::extent_range (extent<S,T> _target):
    m_target (_target)
{ ; }


//-----------------------------------------------------------------------------
template <size_t S, typename T>
typename extent_range<S,T>::iterator
extent_range<S,T>::begin (void) const
{
    return iterator (m_target, util::point<S,T> (0));
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
typename extent_range<S,T>::iterator
extent_range<S,T>::end (void) const
{
    util::point<S,T> cursor (0);
    cursor[S-1] = m_target[S-1];

    return iterator (m_target, cursor);
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
extent_range<S,T>::iterator::iterator (extent<S,T> _target, util::point<S,T> _cursor):
    m_cursor (_cursor),
    m_target (_target)
{
    static_assert (std::is_integral<T>::value, "range stepping size is ill-defined for non-integral types");
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::point<S,T>
extent_range<S,T>::iterator::operator* (void) const
{
    return m_cursor;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
typename extent_range<S,T>::iterator&
extent_range<S,T>::iterator::operator++ (void)
{
    ++m_cursor[0];

    for (size_t i = 0; i < S - 1; ++i) {
        if (m_cursor[i] != m_target[i])
            break;

        m_cursor[i] = 0;
        m_cursor[i+1]++;
        continue;
    }

    return *this;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
bool
extent_range<S,T>::iterator::operator== (const iterator &rhs) const
{
    return m_cursor == rhs.m_cursor;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
bool
extent_range<S,T>::iterator::operator!= (const iterator &rhs) const
{
    return m_cursor != rhs.m_cursor;
}


///////////////////////////////////////////////////////////////////////////////
namespace util::debug  {
    template <size_t S, typename T>
    struct validator<extent<S,T>> {
        static bool is_valid (const extent<S,T> &e)
        {
            return std::all_of (std::begin (e.data),
                                std::end   (e.data),
                                [] (auto i) { return i >= 0; });
        }
    };
}


//-----------------------------------------------------------------------------
#define INSTANTIATE_S_T(S,T)        \
template struct util::extent<S,T>;  \
template bool util::debug::is_valid (const extent<S,T>&); \
template struct util::debug::validator<extent<S,T>>;

#define INSTANTIATE(T)      \
INSTANTIATE_S_T(1,T)        \
INSTANTIATE_S_T(2,T)        \
INSTANTIATE_S_T(3,T)

#define INSTANTIATE_INT(T)          \
template struct util::extent_range<1,T>;  \
template struct util::extent_range<2,T>;  \
template struct util::extent_range<3,T>;

INSTANTIATE( int16_t)
INSTANTIATE( int32_t)
INSTANTIATE( int64_t)

INSTANTIATE(uint16_t)
INSTANTIATE(uint32_t)
INSTANTIATE(uint64_t)

INSTANTIATE(float)
INSTANTIATE(double)

INSTANTIATE_INT( int16_t)
INSTANTIATE_INT( int32_t)
INSTANTIATE_INT( int64_t)

INSTANTIATE_INT(uint16_t)
INSTANTIATE_INT(uint32_t)
INSTANTIATE_INT(uint64_t)
