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
 * Copyright 2010, 2017
 *     Danny Robson <danny@nerdcruft.net>
 */

#include "except.hpp"

#include "../debug.hpp"

using util::win32::error;


///////////////////////////////////////////////////////////////////////////////
error::error (DWORD _code):
    runtime_error (code_string (_code)),
    m_code (_code)
{
    CHECK_NEQ (m_code, (DWORD)ERROR_SUCCESS);
}


//-----------------------------------------------------------------------------
error::error (void):
    error (last_code ())
{ ; }


///////////////////////////////////////////////////////////////////////////////
DWORD
error::code (void) const
{
    return m_code;
}


//-----------------------------------------------------------------------------
DWORD
error::last_code (void)
{
    return GetLastError ();
}


///////////////////////////////////////////////////////////////////////////////
void
error::try_code (void)
{
    try_code (last_code ());
}


//-----------------------------------------------------------------------------
void
error::try_code (DWORD id)
{
    if (__builtin_expect (id != ERROR_SUCCESS, false))
        throw_code (id);
}


//-----------------------------------------------------------------------------
void
error::throw_code (void)
{
    throw_code (last_code ());
}


//-----------------------------------------------------------------------------
void
error::throw_code (DWORD code)
{
    CHECK_NEQ (code, (DWORD)ERROR_SUCCESS);
    throw error (code);
}


//-----------------------------------------------------------------------------
std::string
error::code_string (void)
{
    return code_string (last_code ());
}


//-----------------------------------------------------------------------------
std::string
error::code_string (DWORD code)
{
    char message[256];

    auto res = FormatMessage (
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        code,
        MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),
        message,
        std::size (message),
        NULL
    );

    if (res == 0) {
        error::throw_code ();
    }

    return std::string (message, message + res);
}
