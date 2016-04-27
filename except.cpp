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
 * Copyright 2010 Danny Robson <danny@nerdcruft.net>
 */

#include "./except.hpp"
#include "./debug.hpp"
#include "./platform.hpp"
#include "./cast.hpp"
#include "./types.hpp"

#include <cstring>
#include <cerrno>

using util::errno_error;


///////////////////////////////////////////////////////////////////////////////
/// Construct an errno_error from a given error value. The error value MUST signal an error at
/// construction time.
errno_error::errno_error (int _code):
    runtime_error (strerror (_code)),
    m_code (_code)
{
    CHECK_NEQ (_code, 0);
}


///----------------------------------------------------------------------------
/// Construct an errno_error from the current value of errno. errno MUST signal an error at
/// construction time.
errno_error::errno_error ():
    errno_error (last_code ())
{
    CHECK_NEQ (m_code, 0);
}


///////////////////////////////////////////////////////////////////////////////
int
errno_error::last_code (void)
{
    return errno;
}


//-----------------------------------------------------------------------------
int
errno_error::code (void) const
{
    return m_code;
}


///////////////////////////////////////////////////////////////////////////////
/// Throw an errno_error exception if errno currently signals an error.
void
errno_error::try_code (void)
{
    try_code (last_code ());
}


///----------------------------------------------------------------------------
/// Throw an errno_error exception if 'code' represents an error.
void
errno_error::try_code (int code)
{
    if (__builtin_expect (code != 0, false))
        throw errno_error(code);
}


///----------------------------------------------------------------------------
void
errno_error::throw_code (void)
{
    throw_code (last_code ());
}


///----------------------------------------------------------------------------
void
errno_error::throw_code (int code)
{
    CHECK_NEQ (code, 0);
    throw errno_error (code);
}


///////////////////////////////////////////////////////////////////////////////
#if defined(PLATFORM_WIN32)

using util::win32_error;


win32_error::win32_error (DWORD _id):
    runtime_error ("Win32 error"),
    id (_id)
{
    CHECK_NEQ (id, (DWORD)ERROR_SUCCESS);
}


//-----------------------------------------------------------------------------
win32_error::win32_error (void):
    runtime_error ("Win32 error"),
    id (GetLastError ())
{
    CHECK_NEQ (id, (DWORD)ERROR_SUCCESS);
}


///////////////////////////////////////////////////////////////////////////////
void
win32_error::try_code (void)
{
    try_code (GetLastError ());
}


//-----------------------------------------------------------------------------
void
win32_error::try_code (DWORD id)
{
    if (__builtin_expect (id != ERROR_SUCCESS, false))
        throw_code (id);
}


//-----------------------------------------------------------------------------
void
win32_error::throw_code (void)
{
    throw_code (GetLastError ());
}


//-----------------------------------------------------------------------------
void
win32_error::throw_code (DWORD id)
{
    CHECK_NEQ (id, (DWORD)ERROR_SUCCESS);
    throw win32_error (id);
}


//-----------------------------------------------------------------------------
std::string
win32_error::code_string (void)
{
    return code_string (GetLastError ());
}


//-----------------------------------------------------------------------------
std::string
win32_error::code_string (DWORD code)
{
    char message[256];

    auto res = FormatMessage (
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        code,
        MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),
        message,
        elems (message),
        NULL
    );

    if (res == 0) {
        win32_error::throw_code ();
    }

    return std::string (message, message + res);
}
#endif
