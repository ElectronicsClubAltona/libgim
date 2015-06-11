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
std::vector<char>
util::slurp (const boost::filesystem::path& path)  {
    fd out (path, ACCESS_READ);

    // Calculate the total file size
    off_t size = lseek (out, 0, SEEK_END);
    if (size == (off_t)-1)
        throw errno_error();

    if (lseek (out, 0, SEEK_SET) == (off_t)-1)
        throw errno_error ();

    // Allocate a buffer, and keep reading until it's full. We provide a null
    // padding at the tail as a 'just in case' measure for string manipulation.
    std::vector<char> buffer (size + 1);
    buffer.data ()[size] = '\0';

    CHECK_GE (size, 0);
    size_t remaining = (size_t)size;
    char *cursor = buffer.data ();

    while (remaining) {
        ssize_t consumed = ::read (out, cursor, remaining);
        if (consumed == -1)
            throw errno_error();
        CHECK_GT (        consumed, 0);
        CHECK_LE ((size_t)consumed, remaining);

        remaining -= (size_t)consumed;
        cursor    += consumed;
    }

    return buffer;
}

//----------------------------------------------------------------------------
template <typename T>
void
util::write (const boost::filesystem::path &path, const T *data, size_t len) {
    CHECK_GT (len, 0);
    CHECK (data);

    fd out (path, ACCESS_WRITE);
    const char *cursor = reinterpret_cast<const char*> (data);
    size_t remaining   = len * sizeof (T);

    while (remaining) {
        ssize_t consumed = ::write (out, cursor, remaining);
        if (consumed < 0)
            errno_error::throw_code ();

        remaining -= sign_cast<size_t> (consumed);
        cursor    += sign_cast<size_t> (consumed);
    }
}


template void util::write<char> (const boost::filesystem::path&, const char*,    size_t);
template void util::write<uint8_t> (const boost::filesystem::path&, const uint8_t*, size_t);


//----------------------------------------------------------------------------
fd::fd (int _fd):
    m_fd (_fd)
{
    if (_fd < 0)
        throw std::invalid_argument ("invalid descriptor");
}


fd::fd (const boost::filesystem::path &path, access_t access):
#ifdef PLATFORM_WIN32
    // Windows requires the use of 'string ()' to convert to char datatype
    // rather than the typical wchar
    m_fd (open (path.string ().c_str (), access_to_cflags (access) | O_BINARY, 0660))
#else
    m_fd (open (path.native ().c_str (), access_to_cflags (access), 0660))
#endif
{
    if (m_fd < 0)
        util::errno_error::throw_code ();
}


fd::~fd () {
    CHECK (m_fd >= 0);
    close (m_fd);
}


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
