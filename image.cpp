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
util::image::buffer<T>::buffer (util::extent2u _size):
    buffer<T> (_size.w, _size.h)
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
util::image::buffer<T>::buffer (size_t _w, size_t _h, size_t _s):
    w (_w),
    h (_h),
    s (_s),
    m_data (std::make_unique<T[]> (_h * _s))
{
    CHECK_NEQ (w * h, 0);
    CHECK_GE (s, w);
}


//-----------------------------------------------------------------------------
template <typename T>
util::image::buffer<T>::buffer (size_t _w, size_t _h):
    buffer<T> (_w, _h, _w)
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
util::image::buffer<T>::buffer (size_t _w,
                                size_t _h,
                                size_t _s,
                                std::unique_ptr<T[]> &&_data):
    w (_w),
    h (_h),
    s (_s),
    m_data (std::move (_data))
{
    CHECK_NEQ (w * h, 0);
    CHECK_GE (s, w);
}


//-----------------------------------------------------------------------------
template <typename T>
template <typename U>
util::image::buffer<U>
util::image::buffer<T>::alloc (void) const
{
    return buffer<U> (w, h, s);
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
    CHECK_LT (p.x, w);
    CHECK_LT (p.y, h);

    return begin ()[offset (p)];
}


//-----------------------------------------------------------------------------
template <typename T>
T&
buffer<T>::operator[] (point<2,size_t> p)
{
    CHECK_LT (p.x, w);
    CHECK_LT (p.y, h);

    return begin ()[offset (p)];
}


//-----------------------------------------------------------------------------
template <typename T>
const T&
buffer<T>::operator[] (size_t idx) const
{
    CHECK_LT (idx, h * s);

    return begin ()[idx];
}


//-----------------------------------------------------------------------------
template <typename T>
T&
buffer<T>::operator[] (size_t idx)
{
    CHECK_LT (idx, h * s);

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
    return begin () + h * s;
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
    return cbegin () + h * s;
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
