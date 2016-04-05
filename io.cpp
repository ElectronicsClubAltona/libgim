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
#include "cast.hpp"

#include <cstdio>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <boost/filesystem.hpp>
#include <stdexcept>


using namespace util;


//----------------------------------------------------------------------------
std::vector<char>
util::slurp (const boost::filesystem::path& path)  {
    fd out (path, O_RDONLY | O_BINARY);

    // Calculate the total file size
    off_t size = lseek (out, 0, SEEK_END);
    if (size == (off_t)-1)
        throw errno_error();

    if (lseek (out, 0, SEEK_SET) == (off_t)-1)
        throw errno_error ();

    // Allocate a buffer, and keep reading until it's full.
    std::vector<char> buffer (size);

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


//-----------------------------------------------------------------------------
std::vector<char>
util::slurp (FILE *stream)
{
    // find how much data is in this file
    const int desc = fileno (stream);
    if (desc < 0)
        errno_error::throw_code ();

    struct stat meta;
    if (fstat (desc, &meta) < 0)
        errno_error::throw_code ();

    // allocate a buffer we think is the correct size
    std::vector<char> buf;
    buf.resize (meta.st_size);

    // read as much as possible, then resize to the actual length
    auto res = fread (buf.data (), 1, meta.st_size, stream);
    if (!ferror (stream))
        buf.resize (res);

    CHECK (feof (stream));
    return buf;
}



//-----------------------------------------------------------------------------
void
util::write (const fd &out,
             const void *restrict data,
             size_t bytes)
{
    const char *restrict cursor = reinterpret_cast<const char*> (data);
    size_t remaining = bytes;

    while (remaining) {
        ssize_t consumed = ::write (out, cursor, remaining);
        if (consumed < 0)
            errno_error::throw_code ();

        remaining -= sign_cast<size_t> (consumed);
        cursor    += sign_cast<size_t> (consumed);
    }
}


//////////////////////////////////////////////////////////////////////////////
fd::fd (int _fd):
    m_fd (_fd)
{
    if (_fd < 0)
        throw std::invalid_argument ("invalid descriptor");
}


//-----------------------------------------------------------------------------
fd::fd (const char *path, int flags, mode_t mode):
    m_fd (open (path, flags, mode))
{
    if (m_fd < 0)
        errno_error::throw_code ();
}


//-----------------------------------------------------------------------------
fd::fd (const boost::filesystem::path &path, int flags):
    fd (path.string ().c_str (), flags)
{ ; }


//-----------------------------------------------------------------------------
fd::~fd () {
    CHECK (m_fd >= 0);
    close (m_fd);
}


//////////////////////////////////////////////////////////////////////////////
int
indenter::overflow (int ch) {
    if (m_line_start && ch != '\n')
        m_dest->sputn (m_indent.data (), sign_cast<std::streamsize> (m_indent.size ()));

    m_line_start = ch == '\n';
    return m_dest->sputc (ch);
}


//-----------------------------------------------------------------------------
indenter::indenter (std::streambuf* _dest, size_t _indent)
    : m_dest       (_dest)
    , m_line_start (true)
    , m_indent     (_indent, ' ')
    , m_owner      (NULL)
{ ; }


//-----------------------------------------------------------------------------
indenter::indenter (std::ostream& _dest, size_t _indent)
    : m_dest       (_dest.rdbuf())
    , m_line_start (true)
    , m_indent     (_indent, ' ')
    , m_owner      (&_dest)
{
    m_owner->rdbuf (this);
}


//-----------------------------------------------------------------------------
indenter::~indenter ()
{
    if (m_owner != NULL)
        m_owner->rdbuf (m_dest);
}


//////////////////////////////////////////////////////////////////////////////
scoped_cwd::scoped_cwd ()
{
    m_original.resize (16);
    while (getcwd (&m_original[0], m_original.size ()) == nullptr && errno == ERANGE)
        m_original.resize (m_original.size () * 2);
    errno_error::try_code ();
}


//-----------------------------------------------------------------------------
scoped_cwd::~scoped_cwd ()
{
    if (!chdir (m_original.c_str ()))
        errno_error::throw_code ();
}


///////////////////////////////////////////////////////////////////////////////
path_error::path_error (const boost::filesystem::path &_path):
    runtime_error (format::render ("Unknown path: %!", m_path)),
    m_path (_path)
{ ; }


//-----------------------------------------------------------------------------
const char*
path_error::path (void) const noexcept
{
    return m_path.string ().c_str ();
}
