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
 * Copyright 2010-2014 Danny Robson <danny@nerdcruft.net>
 */

#include "io.hpp"

#include "cast.hpp"
#include "debug.hpp"
#include "posix/fd.hpp"
#include "posix/except.hpp"

#include <sys/stat.h>

using util::detail::posix::mapped_file;


//////////////////////////////////////////////////////////////////////////////
mapped_file::mapped_file (const std::experimental::filesystem::path &path,
                          int fflags,
                          int mflags):
    mapped_file (util::posix::fd (path, fflags), mflags)
{ ; }


//-----------------------------------------------------------------------------
mapped_file::mapped_file (const ::util::posix::fd &src, int mflags)
{
    struct stat meta;
    ::util::posix::error::try_value (fstat (src, &meta));

    m_size = util::cast::sign<size_t> (meta.st_size);
    m_data = (uint8_t *)mmap (NULL, m_size, mflags, MAP_SHARED, src, 0);
    if (m_data == MAP_FAILED)
        ::util::posix::error::throw_code ();
}


//----------------------------------------------------------------------------
mapped_file::~mapped_file ()
{
    CHECK (m_data != NULL);
    munmap (m_data, m_size);
}


//----------------------------------------------------------------------------
size_t
mapped_file::size (void) const
{
    return m_size;
}


//-----------------------------------------------------------------------------
bool
mapped_file::empty (void) const
{
    return size () == 0;
}


//////////////////////////////////////////////////////////////////////////////
uint8_t*
mapped_file::data (void) &
{
    CHECK (m_size > 0);
    CHECK (m_data != NULL);

    return m_data;
}


//----------------------------------------------------------------------------
const uint8_t*
mapped_file::data (void) const &
{
    CHECK (m_size > 0);
    CHECK (m_data != NULL);

    return m_data;
}


//----------------------------------------------------------------------------
uint8_t*
mapped_file::begin (void) &
{
    return data ();
}


//----------------------------------------------------------------------------
uint8_t*
mapped_file::end (void) &
{
    return data () + size ();
}


//-----------------------------------------------------------------------------
const uint8_t*
mapped_file::begin (void) const &
{
    return data ();
}


//-----------------------------------------------------------------------------
const uint8_t*
mapped_file::end (void) const &
{
    return data () + size ();
}


//----------------------------------------------------------------------------
const uint8_t*
mapped_file::cbegin (void) const &
{
    return data ();
}


//----------------------------------------------------------------------------
const uint8_t*
mapped_file::cend (void) const &
{
    return data () + size ();
}

