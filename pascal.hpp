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

#ifndef __UTIL_PASCAL_HPP
#define __UTIL_PASCAL_HPP

#include <cstdlib>
#include <ostream>

namespace util {
    template <typename T>
    class parray {
    public:
        parray (size_t size, T *data);

        T& operator[] (size_t idx);
        const T& operator[] (size_t idx) const;

        T& at (size_t idx);
        const T& at (size_t idx) const;

        T* begin (void);
        T* end   (void);

        const T* cbegin (void) const;
        const T* cend   (void) const;

        const T* data (void) const;
        T* data (void);
        size_t size (void) const;

    private:
        const size_t m_size;
        T *m_data;
    };

    template <typename T>
    std::ostream&
    operator<< (std::ostream&, util::parray<T>);
}


#endif
