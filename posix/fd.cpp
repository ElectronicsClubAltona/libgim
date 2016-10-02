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

#include "./fd.hpp"

#include "../except.hpp"

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using util::posix::fd;


///////////////////////////////////////////////////////////////////////////////
fd::fd (const char *path, int flags):
    m_fd (::open (path, flags))
{
    if (m_fd < 0)
        errno_error::throw_code ();
}


//-----------------------------------------------------------------------------
fd::fd (const char *path, int flags, mode_t mode):
    m_fd (::open (path, flags, mode))
{
    if (m_fd < 0)
        errno_error::throw_code ();
}


///////////////////////////////////////////////////////////////////////////////
fd::fd (fd &&rhs):
    m_fd (-1)
{
    std::swap (m_fd, rhs.m_fd);
}


///////////////////////////////////////////////////////////////////////////////
fd::fd (int _fd):
    m_fd (_fd)
{ ; }


///////////////////////////////////////////////////////////////////////////////
fd
fd::dup (void) const
{
    return dup (m_fd);
}


//-----------------------------------------------------------------------------
fd
fd::dup (int _fd)
{
    auto res = ::dup (_fd);
    if (res < 0)
        errno_error::throw_code ();

    return fd (res);
}


///////////////////////////////////////////////////////////////////////////////
fd::~fd ()
{
    if (m_fd < 0)
        return;

    if (close (m_fd))
        errno_error::throw_code ();
}


///////////////////////////////////////////////////////////////////////////////
struct ::stat
fd::stat (void) const
{
    struct stat buf;
    if (fstat (m_fd, &buf))
        errno_error::throw_code ();
    return buf;
}


///////////////////////////////////////////////////////////////////////////////
fd::operator int (void) const
{
    return m_fd;
}
