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
 * Copyright 2015-2016 Danny Robson <danny@nerdcruft.net>
 */

#include "stack.hpp"

#include "../../debug.hpp"
#include "../../pointer.hpp"
#include "../../cast.hpp"

using util::alloc::raw::stack;


///////////////////////////////////////////////////////////////////////////////
stack::stack (void *begin, void *end):
    m_begin  (reinterpret_cast<char*> (begin)),
    m_end    (reinterpret_cast<char*> (end)),
    m_cursor (reinterpret_cast<char*> (begin))
{
    CHECK_LE (m_begin, m_end);
}


///////////////////////////////////////////////////////////////////////////////
union record {
    using offset_t = uint32_t;

    char     *as_bytes;
    offset_t *as_offset;
};


//-----------------------------------------------------------------------------
constexpr auto MIN_ALIGNMENT = sizeof (record::offset_t);


///////////////////////////////////////////////////////////////////////////////
void*
stack::allocate (size_t bytes)
{
    return allocate (bytes, alignof (std::max_align_t));
}


//-----------------------------------------------------------------------------
void*
stack::allocate (size_t bytes, size_t alignment)
{
    // reserve space at the front of the allocation to record the total
    // allocation size so we can account for alignment if required.
    auto ptr = m_cursor + sizeof (record::offset_t);

    // align the outgoing pointer if required
    alignment = util::max (MIN_ALIGNMENT, alignment);
    ptr = align (ptr, alignment);

    // ensure we haven't overrun our allocated segment
    if (ptr + bytes > m_end)
        throw std::bad_alloc ();

    // use a 'record' struct as a window into the reserved space at the front
    // of the allocation and store the offset to the previous allocation head
    // (from the record struct). allows us to account for alignment.
    record record;
     record.as_bytes  = ptr - sizeof (record::offset_t);
    *record.as_offset = util::cast::lossless <uint32_t> (ptr - m_cursor);

    m_cursor = ptr + bytes;
    return ptr;
}


//-----------------------------------------------------------------------------
void
stack::deallocate (void *ptr, size_t bytes)
{
    return deallocate (ptr, bytes, alignof (std::max_align_t));
}


//-----------------------------------------------------------------------------
void
stack::deallocate (void *_ptr, size_t bytes, size_t alignment)
{
    (void)bytes;

    alignment = util::max (MIN_ALIGNMENT, alignment);
    
    auto ptr = reinterpret_cast<char*> (_ptr);

    record record;
    record.as_bytes = ptr - sizeof (record::offset_t);

    //CHECK_LE (bytes, *record.as_offset);
    CHECK_GE (m_cursor - *record.as_offset, m_begin);

    m_cursor -= bytes;
    m_cursor -= *record.as_offset;
}


//-----------------------------------------------------------------------------
void*
stack::begin (void)
{
    return m_begin;
}


//-----------------------------------------------------------------------------
const void*
stack::begin (void) const
{
    return m_begin;
}


//-----------------------------------------------------------------------------
size_t
stack::offset (const void *_ptr) const
{
    auto ptr = reinterpret_cast<const char*> (_ptr);

    CHECK_GE (ptr, m_begin);
    return ptr - m_begin;
}


//-----------------------------------------------------------------------------
void
stack::reset (void)
{
    m_cursor = m_begin;
}


///////////////////////////////////////////////////////////////////////////////
size_t
stack::capacity (void) const
{
    return m_end - m_begin;
}


//-----------------------------------------------------------------------------
size_t
stack::used (void) const
{
    return m_cursor - m_begin;
}


//-----------------------------------------------------------------------------
size_t
stack::remain (void) const
{
    return capacity () - used ();
}
