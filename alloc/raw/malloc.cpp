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

#include "./malloc.hpp"

#include "../../debug.hpp"

#include <cstdlib>

using util::alloc::raw::malloc;


///////////////////////////////////////////////////////////////////////////////
void*
malloc::allocate (size_t bytes)
{
    return allocate (bytes, alignof (std::max_align_t));
}


//-----------------------------------------------------------------------------
void*
malloc::allocate (size_t bytes, size_t align)
{
    // C malloc guarantees maximal alignment
    CHECK_LE (align, alignof (std::max_align_t));
    (void)align;

    return ::malloc (bytes);
}


//-----------------------------------------------------------------------------
void
malloc::deallocate (void *ptr, size_t bytes)
{
    return deallocate (ptr, bytes, alignof (std::max_align_t));
}


//-----------------------------------------------------------------------------
void
malloc::deallocate (void *ptr, size_t bytes, size_t align)
{
    (void)bytes;
    (void)align;

    ::free (ptr);
}
