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
template <typename T>
util::image::buffer<T>::buffer (util::extentu<2> _size):
    m_size (_size),
    m_stride (1, _size.w),
    m_data (std::make_unique<T[]> (_size.area ()))
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
template <typename U>
util::image::buffer<U>
util::image::buffer<T>::alloc (void) const
{
    return buffer<U> (m_size);
}


//-----------------------------------------------------------------------------
template <typename T, typename U>
static U
rescale (T v)
{
    return v * sizeof (U) / sizeof (T);
}


//-----------------------------------------------------------------------------
template <typename T>
template <typename U>
util::image::buffer<U>
util::image::buffer<T>::clone (void) const
{
    auto out = alloc<U> ();

    std::transform (begin (), end (), out.begin (), renormalise<T,U>);

    return out;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
const T&
buffer<T>::operator[] (point<2,size_t> p) const
{
    CHECK (util::all (p < size ()));

    return begin ()[offset (p)];
}


//-----------------------------------------------------------------------------
template <typename T>
T&
buffer<T>::operator[] (point<2,size_t> p)
{
    CHECK (util::all (p < size ()));

    return begin ()[offset (p)];
}


//-----------------------------------------------------------------------------
template <typename T>
const T&
buffer<T>::operator[] (size_t idx) const
{
    CHECK_LT (idx, size ());

    return begin ()[idx];
}


//-----------------------------------------------------------------------------
template <typename T>
T&
buffer<T>::operator[] (size_t idx)
{
    CHECK_LT (idx, size ());

    return begin ()[idx];
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
T*
buffer<T>::data (void)
{
    return begin ();
}


//-----------------------------------------------------------------------------
template <typename T>
T*
buffer<T>::begin (void)
{
    return m_data.get ();
}


//-----------------------------------------------------------------------------
template <typename T>
T*
buffer<T>::end (void)
{
    return begin () + m_size.back () * m_stride.back ();
}


//-----------------------------------------------------------------------------
template <typename T>
const T*
buffer<T>::begin (void) const
{
    return cbegin ();
}


//-----------------------------------------------------------------------------
template <typename T>
const T*
buffer<T>::end (void) const
{
    return cend ();
}


//-----------------------------------------------------------------------------
template <typename T>
const T*
buffer<T>::data (void) const
{
    return begin ();
}


//-----------------------------------------------------------------------------
template <typename T>
const T*
buffer<T>::cbegin (void) const
{
    return m_data.get ();
}


//-----------------------------------------------------------------------------
template <typename T>
const T*
buffer<T>::cend (void) const
{
    return cbegin () + m_size.back () * m_stride.back ();
}


///////////////////////////////////////////////////////////////////////////////
template struct util::image::buffer<char>;
template struct util::image::buffer<uint8_t>;
template struct util::image::buffer<uint16_t>;
template struct util::image::buffer<uint32_t>;
template struct util::image::buffer< int32_t>;
template struct util::image::buffer<float>;
template struct util::image::buffer<double>;

template util::image::buffer<char> util::image::buffer<char>::alloc (void) const;

template util::image::buffer<uint8_t> util::image::buffer<uint8_t>::alloc (void) const;
template util::image::buffer<uint8_t> util::image::buffer<uint8_t>::clone (void) const;
template util::image::buffer<uint8_t> util::image::buffer<float>::clone (void) const;
template util::image::buffer<uint8_t> util::image::buffer<double>::clone (void) const;

template util::image::buffer<float>    util::image::buffer<float>::alloc (void) const;
template util::image::buffer<uint32_t> util::image::buffer<float>::alloc (void) const;
template util::image::buffer< int32_t> util::image::buffer<float>::alloc (void) const;

template util::image::buffer<float> util::image::buffer<float>::clone (void) const;

template util::image::buffer<uint32_t> util::image::buffer<uint32_t>::alloc (void) const;
