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

#ifndef CRUFT_UTIL_ALLOC_RAW_MALLOC_HPP
#define CRUFT_UTIL_ALLOC_RAW_MALLOC_HPP

#include <cstddef>


namespace util::alloc::raw {
    class malloc {
    public:
        void* allocate (size_t bytes);
        void* allocate (size_t bytes, size_t align);

        void  deallocate (void *ptr, size_t bytes);
        void  deallocate (void *ptr, size_t bytes, size_t align);
    };
}


#endif