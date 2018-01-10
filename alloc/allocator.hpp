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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_ALLOC_ALLOCATOR_HPP
#define __UTIL_ALLOC_ALLOCATOR_HPP

#include <cstddef>

// C++11 allocator concept conformant allocator adaptor, going from our
// allocator interface to that of the STL and friends.
namespace util::alloc {
    template <class B, class T>
    class allocator {
    public:
        typedef T value_type;

        template <typename ...Args>
        explicit allocator (Args&& ...args);

        T* allocate (size_t count);
        void deallocate (T*, size_t count);

    private:
        B &m_backing;
    };
}

#include "allocator.ipp"

#endif
