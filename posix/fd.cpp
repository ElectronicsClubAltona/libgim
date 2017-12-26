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

#include "fd.hpp"

#include "except.hpp"

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using util::posix::fd;


///////////////////////////////////////////////////////////////////////////////
fd::fd (const std::experimental::filesystem::path &path, int flags):
    fd (path, flags, 0666)
{ ; }


//-----------------------------------------------------------------------------
fd::fd (const std::experimental::filesystem::path &path, int flags, mode_t mode):
    m_fd (error::try_value (::open (path.c_str (), flags, mode)))
{ ; }


///////////////////////////////////////////////////////////////////////////////
fd::fd (fd &&rhs):
    m_fd (-1)
{
    std::swap (m_fd, rhs.m_fd);
}


//-----------------------------------------------------------------------------
fd&
fd::operator= (fd &&rhs)
{
    close ();
    std::swap (m_fd, rhs.m_fd);
    return *this;
}


//-----------------------------------------------------------------------------
fd&
fd::operator= (int rhs)
{
    reset (rhs);
    return *this;
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
    return fd {
        error::try_value (::dup (_fd))
    };
}


///////////////////////////////////////////////////////////////////////////////
fd::~fd ()
{
    if (m_fd < 0)
        return;
    close ();
}


///////////////////////////////////////////////////////////////////////////////
struct ::stat
fd::stat (void) const
{
    struct stat buf;
    error::try_value (fstat (m_fd, &buf));
    return buf;
}


///////////////////////////////////////////////////////////////////////////////
void
fd::close (void)
{
    error::try_value (::close (m_fd));
    m_fd = -1;
}


//-----------------------------------------------------------------------------
void
fd::reset (void)
{
    if (m_fd >= 0) {
        close ();
        m_fd = -1;
    }
}


//-----------------------------------------------------------------------------
void
fd::reset (int rhs)
{
    if (m_fd >= 0)
        close ();
    m_fd = rhs;
}


//-----------------------------------------------------------------------------
int
fd::release (void)
{
    int tmp = m_fd;
    m_fd = -1;
    return tmp;
}


///////////////////////////////////////////////////////////////////////////////
ssize_t
fd::read (void *buffer, size_t count)
{
    return error::try_value (
        ::read (m_fd, buffer, count)
    );
}


///////////////////////////////////////////////////////////////////////////////
ssize_t
fd::write (const void *buffer, size_t count)
{
    return error::try_value (
        ::write (m_fd, buffer, count)
    );
}


///////////////////////////////////////////////////////////////////////////////
off_t
fd::lseek (off_t offset, int whence)
{
    return error::try_value (
        ::lseek (m_fd, offset, whence)
    );
}


///////////////////////////////////////////////////////////////////////////////
fd::operator int (void) const
{
    return m_fd;
}
