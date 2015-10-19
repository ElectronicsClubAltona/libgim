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

#ifdef __UTIL_IMAGE_IPP
#error
#endif
#define __UTIL_IMAGE_IPP

namespace util { namespace image {
    //-------------------------------------------------------------------------
    template <typename T>
    constexpr extent2u
    buffer<T>::extent (void) const
    {
        return m_size;
    }


    //-------------------------------------------------------------------------
    template <typename T>
    constexpr vector2u
    buffer<T>::stride (void) const
    {
        return m_stride;
    }


    //-------------------------------------------------------------------------
    template <typename T>
    constexpr size_t
    buffer<T>::offset (point<2,size_t> p) const
    {
        return dot (stride (), p);
    }


    //-------------------------------------------------------------------------
    template <typename T>
    constexpr size_t
    buffer<T>::size (void) const
    {
        return extent ().back () * stride ().back ();
    }


    //-------------------------------------------------------------------------
    template <typename T>
    constexpr bool
    buffer<T>::is_packed (void) const
    {
        return stride ().back () == extent ().back ();
    }
} }
