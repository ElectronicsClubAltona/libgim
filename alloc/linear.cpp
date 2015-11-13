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

#include "./linear.hpp"

#include "../pointer.hpp"
#include "../debug.hpp"

using util::alloc::linear;


///////////////////////////////////////////////////////////////////////////////
linear::linear (void *begin, void *end):
    m_begin  (reinterpret_cast<char*> (begin)),
    m_end    (reinterpret_cast<char*> (end)),
    m_cursor (reinterpret_cast<char*> (begin))
{
    CHECK_LE (begin, end);
}


///////////////////////////////////////////////////////////////////////////////
void*
linear::allocate (size_t bytes, size_t alignment)
{
    auto ptr = align (m_cursor, alignment);
    if (ptr + bytes > m_end)
        throw std::bad_alloc ();

    m_cursor = ptr + bytes;
    return ptr;
}


//-----------------------------------------------------------------------------
void
linear::deallocate (void *ptr, size_t bytes, size_t alignment)
{
    (void)ptr;
    (void)bytes;
    (void)alignment;
}


///////////////////////////////////////////////////////////////////////////////
void
linear::reset (void)
{
    m_cursor = m_begin;
}
