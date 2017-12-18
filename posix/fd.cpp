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
    m_fd (::open (path.c_str (), flags, mode))
{
    if (m_fd < 0)
        error::throw_code ();
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
        error::throw_code ();

    return fd (res);
}


///////////////////////////////////////////////////////////////////////////////
fd::~fd ()
{
    if (m_fd < 0)
        return;

    if (close (m_fd))
        error::throw_code ();
}


///////////////////////////////////////////////////////////////////////////////
struct ::stat
fd::stat (void) const
{
    struct stat buf;
    if (fstat (m_fd, &buf))
        error::throw_code ();
    return buf;
}


///////////////////////////////////////////////////////////////////////////////
ssize_t
fd::read (void *buffer, size_t count)
{
    auto res = ::read (m_fd, buffer, count);
    if (res == -1)
        error::throw_code ();
    return res;
}

//-----------------------------------------------------------------------------
ssize_t
fd::write (const void *buffer, size_t count)
{
    auto res = ::write (m_fd, buffer, count);
    if (res == -1)
        error::throw_code ();
    return res;
}


///////////////////////////////////////////////////////////////////////////////
off_t
fd::lseek (off_t offset, int whence)
{
    auto res = ::lseek (m_fd, offset, whence);
    if (res == -1)
        error::throw_code ();
    return res;
}


///////////////////////////////////////////////////////////////////////////////
fd::operator int (void) const
{
    return m_fd;
}
