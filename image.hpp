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

#ifndef __UTIL_IMAGE_HPP
#define __UTIL_IMAGE_HPP

#include "extent.hpp"
#include "point.hpp"

#include <cstdint>
#include <cstdlib>
#include <memory>


namespace util {
    namespace image {
        template <typename T>
        struct buffer {
            typedef T value_type;

            buffer (util::extent2u);
            buffer (size_t w, size_t h);
            buffer (size_t w, size_t h, size_t s);
            buffer (size_t w, size_t h, size_t s, std::unique_ptr<T[]> &&data);

            buffer (const buffer<T>&) = delete;
            buffer (buffer<T> &&) = default;

            void fill (T);

            template <typename U>
            buffer<U> alloc (void) const;

            template <typename U>
            buffer<U> clone (void) const;

            buffer<T> downsample (float factor) const;

            size_t w;
            size_t h;
            size_t s;

            T  operator[] (point<2,size_t>) const;
            T& operator[] (point<2,size_t>);

            T  operator[] (size_t) const;
            T& operator[] (size_t);

            T* begin (void);
            T* end   (void);
            T* data  (void);

            const T* begin (void) const;
            const T* end   (void) const;
            const T* data  (void) const;

        private:
            std::unique_ptr<T[]> m_data;
        };
    }
}

#endif
