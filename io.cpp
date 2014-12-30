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
#include "platform.hpp"
#include "types/casts.hpp"

#include <cstdio>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <boost/filesystem.hpp>
#include <stdexcept>


using namespace std;
using namespace util;

//----------------------------------------------------------------------------
static int
access_to_cflags (access_t a) {
    int flags = 0;

    if ((a & ACCESS_READWRITE) == ACCESS_READWRITE) {
        flags = O_RDWR | O_CREAT;
    } else if (a & ACCESS_READ) {
        flags = O_RDONLY;
    } else if (a & ACCESS_WRITE) {
        flags = O_WRONLY | O_CREAT;
    }

    a = static_cast<access_t> (a & ~ACCESS_READWRITE);

    return flags;
}

//----------------------------------------------------------------------------
std::unique_ptr<char []>
util::slurp (const boost::filesystem::path& path)  {
    fd_ref fd(path, ACCESS_READ);

    // Calculate the total file size
    off_t size = lseek (fd, 0, SEEK_END);
    if (size == (off_t)-1)
        throw errno_error();

    if (lseek (fd, 0, SEEK_SET) == (off_t)-1)
        throw errno_error ();

    // Allocate a buffer, and keep reading until it's full. We provide a null
    // padding at the tail as a 'just in case' measure for string manipulation.
    unique_ptr <char []> buffer (new char[size + 1]);
    buffer.get()[size] = '\0';

    CHECK_HARD (size >= 0);
    size_t remaining = (size_t)size;
    char *cursor = buffer.get();

    while (remaining) {
        ssize_t consumed = ::read (fd, cursor, remaining);
        if (consumed == -1)
            throw errno_error();
        CHECK_HARD (        consumed >  0);
        CHECK_HARD ((size_t)consumed <= remaining);

        remaining -= (size_t)consumed;
        cursor    += consumed;
    }

    return buffer;
}

//----------------------------------------------------------------------------
template <typename T>
void
util::write (const boost::filesystem::path &path, const T *data, size_t len) {
    CHECK_SOFT (len > 0);
    CHECK_HARD (data);

    fd_ref fd (path, ACCESS_WRITE);
    const char *cursor = reinterpret_cast<const char*> (data);
    size_t remaining   = len * sizeof (T);

    while (remaining) {
        ssize_t consumed = ::write (fd, cursor, remaining);
        if (consumed < 0)
            errno_error::throw_code ();

        remaining -= sign_cast<size_t> (consumed);
        cursor    += sign_cast<size_t> (consumed);
    }
}

template void write<char> (const boost::filesystem::path&, const char*,    size_t);
template void write<uint8_t> (const boost::filesystem::path&, const uint8_t*, size_t);


//----------------------------------------------------------------------------
fd_ref::fd_ref (int _fd):
    fd (_fd)
{
    if (fd < 0)
        throw std::invalid_argument ("invalid descriptor");
}


fd_ref::fd_ref (const boost::filesystem::path &path, access_t access):
#ifdef PLATFORM_WIN32
    // Windows requires the use of 'string ()' to convert to char datatype
    // rather than the typical wchar
    fd (open (path.string ().c_str (), access_to_cflags (access) | O_BINARY, 0660))
#else
    fd (open (path.native ().c_str (), access_to_cflags (access), 0660))
#endif
{
    if (fd < 0)
        util::errno_error::throw_code ();
}


fd_ref::~fd_ref () {
    CHECK (fd >= 0);
    close (fd);
}


fd_ref::operator int (void) const
    { return fd; }



//----------------------------------------------------------------------------
int
indenter::overflow (int ch) {
    if (m_line_start && ch != '\n')
        m_dest->sputn (m_indent.data (), sign_cast<std::streamsize> (m_indent.size ()));

    m_line_start = ch == '\n';
    return m_dest->sputc (ch);
}


indenter::indenter (std::streambuf* _dest, size_t _indent)
    : m_dest       (_dest)
    , m_line_start (true)
    , m_indent     (_indent, ' ')
    , m_owner      (NULL)
{ ; }


indenter::indenter (std::ostream& _dest, size_t _indent)
    : m_dest       (_dest.rdbuf())
    , m_line_start (true)
    , m_indent     (_indent, ' ')
    , m_owner      (&_dest)
{
    m_owner->rdbuf (this);
}


indenter::~indenter ()
{
    if (m_owner != NULL)
        m_owner->rdbuf (m_dest);
}

//----------------------------------------------------------------------------
scoped_cwd::scoped_cwd ():
    m_original(boost::filesystem::canonical (getcwd (nullptr, 0)))
{ ; }


scoped_cwd::~scoped_cwd () {
    set_cwd (m_original);
}


void
util::set_cwd (const boost::filesystem::path &path) {
    CHECK (path.string ().size () > 0);

    if (chdir (path.string ().c_str ()) != 0)
        throw errno_error ();
}
