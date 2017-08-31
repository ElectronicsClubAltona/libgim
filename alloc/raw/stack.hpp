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

#ifndef CRUFT_UTIL_ALLOC_RAW_STACK_HPP
#define CRUFT_UTIL_ALLOC_RAW_STACK_HPP

#include <cstddef>


namespace util::alloc::raw {
    // allocate memory from a buffer in a stacklike manner. deallocation that
    // is not correctly ordered has undefined (read 'bad') results.
    class stack {
    public:
        stack (const stack&) = delete;
        stack (stack&&) = delete;
        stack& operator= (const stack&) = delete;
        stack& operator= (stack&&) = delete;

        stack (void *begin, void *end);

        void *allocate  (size_t bytes, size_t alignment);
        void *allocate  (size_t bytes);

        void deallocate (void *ptr, size_t bytes);
        void deallocate (void *ptr, size_t bytes, size_t alignment);

        void* begin (void);
        const void* begin (void) const;
        size_t offset (const void*) const;

        void reset (void);

        size_t capacity (void) const;
        size_t used     (void) const;
        size_t remain   (void) const;

    private:
        char *m_begin, *m_end, *m_cursor;
    };
}

#endif
