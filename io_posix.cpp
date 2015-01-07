/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2010-2014 Danny Robson <danny@nerdcruft.net>
 */

#include "io.hpp"

#include "debug.hpp"
#include "except.hpp"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

using util::detail::posix::mapped_file;

//----------------------------------------------------------------------------
mapped_file::mapped_file (const boost::filesystem::path &_path, access_t _access):
    m_fd (_path, _access)
{ load_fd (_access); }


//----------------------------------------------------------------------------
mapped_file::~mapped_file () {
    CHECK (m_data != NULL);
    munmap (m_data, m_size);
}


//----------------------------------------------------------------------------
int
mapped_file::access_to_flags (access_t a) {
    int flags = 0;

    if (a & ACCESS_READ)
        flags |= PROT_READ;

    if (a & ACCESS_WRITE)
        flags |= PROT_WRITE;

    return flags;
}


//----------------------------------------------------------------------------
void
mapped_file::load_fd (access_t access) {
    struct stat meta;
    if (fstat (m_fd, &meta) < 0)
        throw errno_error ();

    m_size = (size_t)meta.st_size;
    m_data = (uint8_t *)mmap (NULL, m_size, access_to_flags (access), MAP_SHARED, m_fd, 0);
    if (m_data == MAP_FAILED)
        throw errno_error ();
}


//----------------------------------------------------------------------------
size_t
mapped_file::size (void) const {
    CHECK (m_size > 0);
    CHECK (m_data != NULL);

    return m_size;
}


//----------------------------------------------------------------------------
uint8_t*
mapped_file::data (void) {
    CHECK (m_size > 0);
    CHECK (m_data != NULL);

    return m_data;
}


//----------------------------------------------------------------------------
const uint8_t*
mapped_file::data (void) const {
    CHECK (m_size > 0);
    CHECK (m_data != NULL);

    return m_data;
}


//----------------------------------------------------------------------------
uint8_t*
mapped_file::begin (void) {
    return data ();
}


//----------------------------------------------------------------------------
uint8_t*
mapped_file::end (void) {
    return data () + size ();
}


//----------------------------------------------------------------------------
const uint8_t*
mapped_file::cbegin (void) const {
    return data ();
}


//----------------------------------------------------------------------------
const uint8_t*
mapped_file::cend (void) const {
    return data () + size ();
}

