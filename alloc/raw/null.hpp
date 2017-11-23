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

#ifndef CRUFT_UTIL_ALLOC_RAW_NULL_HPP
#define CRUFT_UTIL_ALLOC_RAW_NULL_HPP

#include <cstddef>


namespace util::alloc::raw {
    // allocator that always fails, throwing bad_alloc. deallocate will
    // succeed with nullptr as with delete, but is undefined with other values
    // (it is likely to at least assert).
    class null {
    public:
        null () = default;
        null (const null&) = delete;
        null& operator= (const null&) = delete;

        void* allocate (size_t bytes);
        void* allocate (size_t bytes, size_t align);
        void deallocate (void *ptr, size_t bytes);
        void deallocate (void *ptr, size_t bytes, size_t align);

        void* begin (void);
        const void* begin (void) const;
        size_t offset (const void*) const;

        void reset (void);

        size_t capacity (void) const;
        size_t used     (void) const;
        size_t remain   (void) const;
    };
}

#endif
