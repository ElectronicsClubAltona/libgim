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

#ifndef __UTIL_IMAGE_BUFFER_HPP
#define __UTIL_IMAGE_BUFFER_HPP

#include "../extent.hpp"
#include "../point.hpp"

#include <cstdint>
#include <cstdlib>
#include <memory>


namespace util { namespace image {
    template <size_t C, typename T>
    struct buffer {
        typedef T value_type;

        //---------------------------------------------------------------------
        buffer (util::extentu<2>);
        buffer (util::extentu<2>, std::unique_ptr<T[]> &&data);

        buffer (const buffer&) = delete;
        buffer (buffer &&) = default;
        buffer& operator= (const buffer&) = default;
        buffer& operator= (buffer&&) = default;

        //---------------------------------------------------------------------
        /// allocate and return a buffer of the same dimensions. contents are undefined.
        template <typename U = T> buffer<C,U> alloc (void) const;

        /// allocate and return a buffer with the same contents
        template <typename U = T> buffer<C,U> clone (void) const;
        template <typename U> buffer<C,U> cast (void) const { return clone<U> (); }

        //---------------------------------------------------------------------
        constexpr extent2u extent (void) const;
        constexpr vector2u stride (void) const;
        constexpr size_t   size   (void) const; // elements allocated

        constexpr bool is_packed (void) const;

        //---------------------------------------------------------------------
        constexpr size_t offset (point<2,size_t>) const;

        const T& operator[] (point<2,size_t>) const;
        T& operator[] (point<2,size_t>);

        const T& operator[] (size_t) const;
        T& operator[] (size_t);

        //---------------------------------------------------------------------
        T* begin (void);
        T* end   (void);
        T* data  (void);

        const T* begin (void) const;
        const T* end   (void) const;
        const T* data  (void) const;

        const T* cbegin (void) const;
        const T* cend   (void) const;

    private:
        util::extent2u m_size;
        util::vector2u m_stride;
        std::unique_ptr<T[]> m_data;
    };
} }

#include "./buffer.ipp"

#endif
