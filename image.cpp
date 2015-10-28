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

#include "image.hpp"

#include "debug.hpp"

using util::image::buffer;


//-----------------------------------------------------------------------------
template <size_t C, typename T>
buffer<C,T>::buffer (util::extentu<2> _size):
    m_size (_size),
    m_stride (C, C * _size.w),
    m_data (std::make_unique<T[]> (_size.area () * C))
{ ; }


//-----------------------------------------------------------------------------
template <size_t C, typename T>
template <typename U>
buffer<C,U>
buffer<C,T>::alloc (void) const
{
    return buffer<C,U> (m_size);
}


//-----------------------------------------------------------------------------
template <typename T, typename U>
static U
rescale (T v)
{
    return v * sizeof (U) / sizeof (T);
}


//-----------------------------------------------------------------------------
template <size_t C, typename T>
template <typename U>
util::image::buffer<C,U>
util::image::buffer<C,T>::clone (void) const
{
    auto out = alloc<U> ();

    auto func = renormalise<T,U>;
    std::transform (begin (), end (), out.begin (), func);

    return out;
}


///////////////////////////////////////////////////////////////////////////////
template <size_t C, typename T>
const T&
buffer<C,T>::operator[] (point<2,size_t> p) const
{
    CHECK (util::all (p < extent ()));

    return begin ()[offset (p)];
}


//-----------------------------------------------------------------------------
template <size_t C, typename T>
T&
buffer<C,T>::operator[] (point<2,size_t> p)
{
    CHECK (util::all (p < extent ()));

    return begin ()[offset (p)];
}


//-----------------------------------------------------------------------------
template <size_t C, typename T>
const T&
buffer<C,T>::operator[] (size_t idx) const
{
    CHECK_LT (idx, size ());

    return begin ()[idx];
}


//-----------------------------------------------------------------------------
template <size_t C, typename T>
T&
buffer<C,T>::operator[] (size_t idx)
{
    CHECK_LT (idx, size ());

    return begin ()[idx];
}


///////////////////////////////////////////////////////////////////////////////
template <size_t C, typename T>
T*
buffer<C,T>::data (void)
{
    return begin ();
}


//-----------------------------------------------------------------------------
template <size_t C, typename T>
T*
buffer<C,T>::begin (void)
{
    return m_data.get ();
}


//-----------------------------------------------------------------------------
template <size_t C, typename T>
T*
buffer<C,T>::end (void)
{
    return begin () + m_size.back () * m_stride.back ();
}


//-----------------------------------------------------------------------------
template <size_t C, typename T>
const T*
buffer<C,T>::begin (void) const
{
    return cbegin ();
}


//-----------------------------------------------------------------------------
template <size_t C, typename T>
const T*
buffer<C,T>::end (void) const
{
    return cend ();
}


//-----------------------------------------------------------------------------
template <size_t C, typename T>
const T*
buffer<C,T>::data (void) const
{
    return begin ();
}


//-----------------------------------------------------------------------------
template <size_t C, typename T>
const T*
buffer<C,T>::cbegin (void) const
{
    return m_data.get ();
}


//-----------------------------------------------------------------------------
template <size_t C, typename T>
const T*
buffer<C,T>::cend (void) const
{
    return cbegin () + m_size.back () * m_stride.back ();
}


///////////////////////////////////////////////////////////////////////////////

#define INSTANTIATE_C_T_U(C,T,U)                                                \
template util::image::buffer<C,U> util::image::buffer<C,T>::alloc (void) const; \
template util::image::buffer<C,U> util::image::buffer<C,T>::clone (void) const; \
template util::image::buffer<C,U> util::image::buffer<C,T>::cast  (void) const;

#define INSTANTIATE_C_T(C,T)    \
template struct util::image::buffer<C,T>;   \
INSTANTIATE_C_T_U(C,T,uint8_t)  \
INSTANTIATE_C_T_U(C,T,uint16_t) \
INSTANTIATE_C_T_U(C,T,uint32_t) \
INSTANTIATE_C_T_U(C,T,uint64_t) \
INSTANTIATE_C_T_U(C,T,float)    \
INSTANTIATE_C_T_U(C,T,double)


#define INSTANTIATE_C(C)        \
INSTANTIATE_C_T(C,uint8_t)      \
INSTANTIATE_C_T(C,uint16_t)     \
INSTANTIATE_C_T(C,uint32_t)     \
INSTANTIATE_C_T(C,uint64_t)     \
INSTANTIATE_C_T(C,float)        \
INSTANTIATE_C_T(C,double)

INSTANTIATE_C(1)
INSTANTIATE_C(2)
INSTANTIATE_C(3)
INSTANTIATE_C(4)
