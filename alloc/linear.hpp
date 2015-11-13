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

#ifndef __UTIL_ALLOC_LINEAR_HPP
#define __UTIL_ALLOC_LINEAR_HPP

#include <cstddef>


namespace util { namespace alloc {
    class linear {
    public:
        linear (const linear&) = delete;
        linear (linear&&) = delete;
        linear& operator= (const linear&) = delete;
        linear& operator= (linear&&) = delete;

        linear (void *begin, void *end);

        void* allocate (size_t bytes, size_t alignment = alignof (std::max_align_t));
        void  deallocate (void *ptr, size_t bytes, size_t alignment = alignof (std::max_align_t));

        void reset (void);

        size_t capacity (void) const;
        size_t size     (void) const;

    protected:
        char *m_begin, *m_end, *m_cursor;
    };
} }

#include "./linear.hpp"

#endif
