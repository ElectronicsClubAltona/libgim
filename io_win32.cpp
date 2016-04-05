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
 * Copyright 2014 Danny Robson <danny@nerdcruft.net>
 */

#include "./io_win32.hpp"

#include "./debug.hpp"
#include "./except.hpp"

#include <windows.h>

using util::detail::win32::mapped_file;


//-----------------------------------------------------------------------------
static DWORD
fflags_to_native (int flags) {
    switch (flags) {
    case O_RDONLY:  return GENERIC_READ;
    case O_WRONLY:  return GENERIC_WRITE;
    case O_RDWR:    return GENERIC_READ | GENERIC_WRITE;
    }

    unreachable ();
}

//-----------------------------------------------------------------------------
mapped_file::mapped_file (const boost::filesystem::path &path,
                          int fflags,
                          int mflags):
    m_data (nullptr, UnmapViewOfFile)
{
    m_file.reset (
        CreateFile (
            path.string ().c_str (),
            fflags_to_native (fflags),
            fflags & O_RDONLY ? FILE_SHARE_READ : 0,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
            0
        )
    );

    if (m_file == INVALID_HANDLE_VALUE)
        win32_error::throw_code ();

    m_mapping.reset (CreateFileMapping (m_file,
                                        nullptr,
                                        access == O_RDONLY ? PAGE_READONLY : PAGE_READWRITE,
                                        0, 0,
                                        nullptr));

    if (m_mapping == INVALID_HANDLE_VALUE)
        win32_error::throw_code ();

    auto view = MapViewOfFile (m_mapping,
                               access == O_RDONLY ? FILE_MAP_READ : FILE_MAP_WRITE,
                               0, 0,
                               0);
    if (view == nullptr)
        win32_error::throw_code ();

    m_data.reset (
        static_cast<unsigned char*> (view)
    );

    m_size = GetFileSize (m_file, nullptr);
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
util::handle::handle():
    fd (INVALID_HANDLE_VALUE)
{ ; }


util::handle::~handle ()
{
    reset (INVALID_HANDLE_VALUE);
}


void
util::handle::reset (HANDLE _handle)
{
    if (fd != INVALID_HANDLE_VALUE)
        if (!CloseHandle (fd))
            win32_error::throw_code ();

    fd = _handle;
}


util::handle::operator HANDLE (void) const
{
    return fd;
}
