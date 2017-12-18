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
 * Copyright 2016 Danny Robson <danny@nerdcruft.net>
 */

#include "map.hpp"

#include "posix/except.hpp"

using util::posix::map;


///////////////////////////////////////////////////////////////////////////////
map::map (size_t size, int prot, int flags):
    m_addr (mmap (nullptr, size, prot, flags, -1, 0)),
    m_size (size)
{
    if (MAP_FAILED == m_addr)
        ::util::posix::error::throw_code ();
}


//-----------------------------------------------------------------------------
map::map (size_t size, int prot, int flags, const fd &src, off_t offset):
    m_addr (mmap (nullptr, size, prot, flags, src, offset)),
    m_size (size)
{
    if (MAP_FAILED == m_addr)
        ::util::posix::error::throw_code ();
}


//-----------------------------------------------------------------------------
map::map (map &&rhs):
    m_addr (MAP_FAILED),
    m_size (rhs.m_size)
{
    std::swap (m_addr, rhs.m_addr);
}


//-----------------------------------------------------------------------------
map::~map ()
{
    if (MAP_FAILED == m_addr)
        return;

    if (munmap (m_addr, m_size))
        ::util::posix::error::throw_code ();
}


//-----------------------------------------------------------------------------
map&
map::operator= (map &&rhs)
{
    std::swap (m_addr, rhs.m_addr);
    std::swap (m_size, rhs.m_size);

    return *this;
}


///////////////////////////////////////////////////////////////////////////////
uint8_t*
map::begin (void)
{
    return static_cast<uint8_t*> (m_addr);
}


//-----------------------------------------------------------------------------
uint8_t*
map::end (void)
{
    return static_cast<uint8_t*> (m_addr) + m_size;
}


///////////////////////////////////////////////////////////////////////////////
void*
map::data (void)
{
    return m_addr;
}


//-----------------------------------------------------------------------------
const void*
map::data (void) const
{
    return m_addr;
}


///////////////////////////////////////////////////////////////////////////////
bool
map::empty (void) const
{
    return size () == 0;
}


//-----------------------------------------------------------------------------
size_t
map::size (void) const
{
    return m_size;
}


//-----------------------------------------------------------------------------
void
map::resize (size_t newsize, resize_t op)
{
    int flags = 0;
    switch (op) {
    case resize_t::MOVE:
        flags = flags | MREMAP_MAYMOVE;
        break;

    case resize_t::NOMOVE:
        break;
    }

    auto res = mremap (m_addr, m_size, newsize, flags);
    if (res == MAP_FAILED)
        ::util::posix::error::throw_code ();
}
