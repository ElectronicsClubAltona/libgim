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
std::unique_ptr<char []>
util::slurp (const boost::filesystem::path& path)  {
    fd_ref fd(path, O_RDONLY);
    
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
void
util::write (const boost::filesystem::path &path, const char *data, size_t len) {
    CHECK_SOFT (len > 0);
    CHECK_HARD (data);

    fd_ref fd (path, O_WRONLY);
    const char *cursor = data;
    size_t remaining   = len;

    while (remaining) {
        ssize_t consumed = ::write (fd, cursor, remaining);
        if (consumed < 0)
            errno_error::throw_code ();

        remaining -= sign_cast<size_t> (consumed);
        cursor    += sign_cast<size_t> (consumed);
    }
}

//----------------------------------------------------------------------------
fd_ref::fd_ref (int _fd):
    fd (_fd)
{
    if (fd < 0)
        throw std::invalid_argument ("invalid descriptor");
}


fd_ref::fd_ref (const boost::filesystem::path &path, int flags):
#ifdef PLATFORM_WIN32
    fd (open (path.native ().c_str (), flags | O_BINARY))
#else
    fd (open (path.native ().c_str (), flags))
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

//----------------------------------------------------------------------------
#if defined(HAVE_MMAP)
#include <sys/mman.h>


mapped_file::mapped_file (const boost::filesystem::path &_path):
    m_fd (_path, O_RDWR)
{ load_fd (); } 


mapped_file::~mapped_file () {
    CHECK (m_data != NULL);
    munmap (m_data, m_size);
}


void
mapped_file::load_fd (void) {
    struct stat meta;
    if (fstat (m_fd, &meta) < 0)
        throw errno_error ();

    m_size = (size_t)meta.st_size;
    m_data = (uint8_t *)mmap (NULL, m_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0);
    if (m_data == MAP_FAILED)
        throw errno_error ();
}


size_t
mapped_file::size (void) const {
    CHECK (m_size > 0);
    CHECK (m_data != NULL);

    return m_size;
}


uint8_t*
mapped_file::data (void) {
    CHECK (m_size > 0);
    CHECK (m_data != NULL);

    return m_data;
}


const uint8_t*
mapped_file::data (void) const {
    CHECK (m_size > 0);
    CHECK (m_data != NULL);

    return m_data;
}


uint8_t*
mapped_file::begin (void) {
    return data ();
}


uint8_t*
mapped_file::end (void) {
    return data () + size ();
}


const uint8_t*
mapped_file::begin (void) const {
    return data ();
}


const uint8_t*
mapped_file::end (void) const {
    return data () + size ();
}

#endif

