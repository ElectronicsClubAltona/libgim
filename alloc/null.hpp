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

#ifndef __UTIL_ALLOC_NULL_HPP
#define __UTIL_ALLOC_NULL_HPP

#include <cstddef>


namespace util { namespace alloc {
    // allocator that always fails, throwing bad_alloc. deallocate will
    // succeed with nullptr as with delete, but is undefined with other values
    // (it is likely to at least assert).
    class null {
    public:
        void* allocate (size_t bytes, size_t align = alignof (std::max_align_t));
        void deallocate (void *ptr, size_t bytes, size_t align = alignof (std::max_align_t));

        void reset (void) { ; }

        constexpr size_t capacity (void) const { return 0u; }
        constexpr size_t used     (void) const { return 0u; }
        constexpr size_t remain   (void) const { return 0u; }
    };
} }

#endif
