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

#include "except.hpp"
#include "debug.hpp"
#include "platform.hpp"

#include <cstring>
#include <cerrno>

using namespace std;
using namespace util;


/// Construct an errno_error from a given error value. The error value MUST signal an error at
/// construction time.
errno_error::errno_error (int _errno):
    runtime_error (strerror (_errno)),
    id            (_errno)
{
    CHECK_NEQ (_errno, 0);
}


/// Construct an errno_error from the current value of errno. errno MUST signal an error at
/// construction time.
errno_error::errno_error ():
    runtime_error (strerror (errno)),
    id            (errno)
{
    CHECK_NEQ (errno, 0);
}


/// Throw an errno_error exception if errno currently signals an error.
void
errno_error::try_code (void)
    { try_code (errno); }


/// Throw an errno_error exception if 'code' represents an error.
void
errno_error::try_code(int code) {
    if (code != 0)
        throw errno_error(code);
}


void
errno_error::throw_code (void)
    { throw_code (errno); }


void
errno_error::throw_code (int code) {
    CHECK_NEQ (code, 0);
    throw errno_error (code);
}


#if defined(PLATFORM_WIN32)
win32_error::win32_error (DWORD _id):
    runtime_error ("Win32 error"),
    id (_id)
{
    CHECK_NEQ (id, ERROR_SUCCESS);
}


win32_error::win32_error (void):
    runtime_error ("Win32 error"),
    id (GetLastError ())
{
    CHECK_NEQ (id, ERROR_SUCCESS);
}


void
win32_error::try_code (void) {
    const auto id = GetLastError ();
    if (id == ERROR_SUCCESS)
        return;

    throw win32_error (id);
}


void
win32_error::throw_code (void) {
    const auto id = GetLastError ();
    CHECK (id != ERROR_SUCCESS);
    throw win32_error (id);
}

#endif
