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

#ifndef CRUFT_UTIL_ALLOC_RAW_LINEAR_HPP
#define CRUFT_UTIL_ALLOC_RAW_LINEAR_HPP

#include <cstddef>
#include <iterator>

namespace util::alloc::raw {
    // allocate progressively across a buffer without concern for deallocation.
    // deallocation is a noop; the only way to free allocations is via reset.
    class linear {
    public:
        linear (const linear&) = delete;
        linear (linear&&) = delete;
        linear& operator= (const linear&) = delete;
        linear& operator= (linear&&) = delete;

        linear (void *begin, void *end);

        template <typename T>
        linear (T &&view):
            linear (std::begin (view), std::end (view))
        { ; }

        void* allocate (size_t bytes);
        void* allocate (size_t bytes, size_t alignment);

        void  deallocate (void *ptr, size_t bytes);
        void  deallocate (void *ptr, size_t bytes, size_t alignment);

        void* base (void);
        const void* base (void) const;
        size_t offset (const void*) const;

        void reset (void);

        size_t capacity (void) const;
        size_t used     (void) const;
        size_t remain   (void) const;

    protected:
        char *m_begin, *m_end, *m_cursor;
    };
}

#endif
