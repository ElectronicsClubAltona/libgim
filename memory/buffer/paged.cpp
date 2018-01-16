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

#include "paged.hpp"

#include "../../cast.hpp"
#include "../../maths.hpp"
#include "../../pointer.hpp"
#include "../../posix/except.hpp"
#include "../system.hpp"

#include <sys/mman.h>

using util::memory::buffer::paged;


///////////////////////////////////////////////////////////////////////////////
paged::paged (size_t bytes, size_t _window):
    m_window (round_up (_window, pagesize ()))
{
    // reserve the address region with no access permissions
    m_begin = reinterpret_cast<char*> (
        mmap (nullptr, bytes, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)
    );

    if (m_begin == MAP_FAILED)
        posix::error::throw_code ();

    // remap the initial window with read/write permissions
    m_cursor = m_begin + round_up (min (m_window, bytes), pagesize ());
    if (MAP_FAILED == mmap (m_begin,
                            m_cursor - m_begin,
                            PROT_READ | PROT_WRITE,
                            MAP_FIXED | MAP_PRIVATE | MAP_ANONYMOUS, -1, 0))
        posix::error::throw_code ();

    // record the nominal end address
    m_end = m_begin + round_up (bytes, pagesize ());
}


//-----------------------------------------------------------------------------
paged::~paged ()
{
    // ignore errors in production; we don't want to double throw.
    auto res = munmap (m_begin, m_end - m_begin);
    (void)res;
    CHECK_ZERO (res);
}


///////////////////////////////////////////////////////////////////////////////
char*
paged::begin (void)&
{
    return m_begin;
}


//-----------------------------------------------------------------------------
char*
paged::end (void)&
{
    return m_end;
}


///////////////////////////////////////////////////////////////////////////////
void
paged::access (char *cursor)
{
    if (cursor < m_cursor)
        release (cursor);
    else
        commit (cursor);
}


//-----------------------------------------------------------------------------
void
paged::commit (char *cursor)
{
    // bail if it's already mapped
    if (cursor <= m_cursor)
        return;

    if (cursor > m_end || cursor < m_begin)
        throw std::out_of_range ("invalid commit cursor");
    
    // bump the request up to page aligned and tack on a little to amortize
    // syscall overheads
    cursor = align (cursor, pagesize ()) + m_window;
    cursor = min (cursor, m_end);

    if (MAP_FAILED == mmap (m_cursor,
                            cursor - m_cursor,
                            PROT_READ | PROT_WRITE,
                            MAP_FIXED | MAP_PRIVATE | MAP_ANONYMOUS,
                            -1, 0))
        posix::error::throw_code ();

    m_cursor = cursor;
}


//-----------------------------------------------------------------------------
void
paged::release (char *desired)
{
    if (desired > m_end || desired < m_begin)
        throw std::out_of_range ("invalid release cursor");

    align (desired, pagesize ());

    // bail if the region is alread unmapped, or if it's not sufficiently
    // behind the current cursor.
    if (desired >= m_cursor || util::cast::sign<size_t> (m_cursor - desired) < m_window)
        return;

    desired += m_window;

    if (MAP_FAILED == mmap (desired,
                            m_end - desired,
                            PROT_NONE,
                            MAP_FIXED | MAP_PRIVATE | MAP_ANONYMOUS,
                            -1, 0))
        posix::error::throw_code ();

    m_cursor = desired;
}


///////////////////////////////////////////////////////////////////////////////
size_t
paged::size (void) const
{
    return m_cursor - m_begin;
}


//-----------------------------------------------------------------------------
size_t
paged::capacity (void) const
{
    return m_end - m_begin;
}


//-----------------------------------------------------------------------------
size_t
paged::window (void) const
{
    return m_window;
}
