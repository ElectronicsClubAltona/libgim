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
 * Copyright 2014 Danny Robson <danny@nerdcruft.net>
 */

#include "io_win32.hpp"

#include "debug.hpp"
#include "except.hpp"

#include <windows.h>

using util::detail::win32::mapped_file;


//-----------------------------------------------------------------------------
static DWORD
access_to_flags (util::access_t a) {
    switch (a) {
        case util::ACCESS_READ:
            return GENERIC_READ;

        case util::ACCESS_WRITE:
            return GENERIC_WRITE;

        default:
            unreachable ();
    }
}

//-----------------------------------------------------------------------------
mapped_file::mapped_file (const boost::filesystem::path &path,
                          access_t access):
    m_data (nullptr, UnmapViewOfFile)
{
    std::cerr << "mapped_file\n";

    m_file.reset (CreateFile (path.string ().c_str (),
                              access_to_flags (access),
                              access == ACCESS_READ ? FILE_SHARE_READ : 0,
                              nullptr,
                              OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
                              0));

    if (m_file == INVALID_HANDLE_VALUE)
        win32_error::throw_code ();
    std::cerr << "mapped_file: file\n";

    m_mapping.reset (CreateFileMapping (m_file,
                                        nullptr,
                                        access == ACCESS_READ ? PAGE_READONLY : PAGE_READWRITE,
                                        0, 0,
                                        nullptr));

    if (m_mapping == INVALID_HANDLE_VALUE)
        win32_error::throw_code ();
    std::cerr << "mapped_file: mapping\n";

    auto view = MapViewOfFile (m_mapping,
                               access == ACCESS_READ ? FILE_MAP_READ : FILE_MAP_WRITE,
                               0, 0,
                               0);
    if (view == nullptr)
        win32_error::throw_code ();
    std::cerr << "mapped_file: view\n";

    m_data.reset (
        static_cast<unsigned char*> (view)
    );

    m_size = GetFileSize (m_file, nullptr);
    std::cout << "size: " << m_size << '\n';
}


//-----------------------------------------------------------------------------
const uint8_t*
mapped_file::data (void) const
{
    return m_data.get ();
}


uint8_t*
mapped_file::data (void)
{
    return m_data.get ();
}


size_t
mapped_file::size (void) const
{
    return m_size;
}


uint8_t*
mapped_file::begin (void)
{
    return data ();
}

uint8_t*
mapped_file::end (void)
{
    return data () + size ();
}


const uint8_t*
mapped_file::cbegin (void) const
{
    return data ();
}

const uint8_t*
mapped_file::cend (void) const
{
    return data () + size ();
}


//-----------------------------------------------------------------------------
util::handle_ref::handle_ref ():
    handle (INVALID_HANDLE_VALUE)
{ ; }


util::handle_ref::~handle_ref ()
{
    reset (INVALID_HANDLE_VALUE);
}


void
util::handle_ref::reset (HANDLE _handle)
{
    if (handle != INVALID_HANDLE_VALUE)
        if (!CloseHandle (handle))
            win32_error::throw_code ();

    handle = _handle;
}


util::handle_ref::operator HANDLE (void) const
{
    return handle;
}
