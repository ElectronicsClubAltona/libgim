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

#include "debug.hpp"

#include "except.hpp"
#include "log.hpp"
#include "except.hpp"

#include <windows.h>
#include <iostream>


///////////////////////////////////////////////////////////////////////////////
void
breakpoint (void)
{
    DebugBreak ();
}


///////////////////////////////////////////////////////////////////////////////
void
await_debugger (void)
{
    // HACK: This is terribly inefficient, but it's not a performance
    // priority for the time being.
    while (!IsDebuggerPresent ())
        Sleep (100);
}


//-----------------------------------------------------------------------------
void
prepare_debugger (void)
{
    if (nullptr == LoadLibrary("exchndl.dll")) {
        auto code = GetLastError ();
        LOG_WARNING("Emergency debugger not loaded: %s", util::win32_error::code_string (code));
    }
}


////////////////////////////////////////////////////////////////////////////////
void
enable_fpe (void)
{
    LOG_WARN ("Non-Linux exception code is broken under current GCC.");
//    _clearfp();
//    unsigned int ignored;
//    _controlfp_s (&ignored, _MCW_EM, _MCW_EM);
}


//-----------------------------------------------------------------------------
void
disable_fpe (void)
{
    LOG_WARN ("Non-Linux exception code is broken under current GCC.");
//    _clearfp();
//    unsigned int ignored;
//    _controlfp_s (&ignored, 0, _MCW_EM);
}


///////////////////////////////////////////////////////////////////////////////
#include <io.h>
#include <fcntl.h>

void
force_console (void)
{
    if (!AttachConsole (ATTACH_PARENT_PROCESS))
        return;

    auto out_handle = GetStdHandle (STD_OUTPUT_HANDLE);
    auto err_handle = GetStdHandle (STD_ERROR_HANDLE);
    auto in_handle  = GetStdHandle (STD_INPUT_HANDLE);

    auto out_fd = _open_osfhandle (reinterpret_cast<intptr_t> (out_handle), _O_TEXT);
    auto err_fd = _open_osfhandle (reinterpret_cast<intptr_t> (err_handle), _O_TEXT);
    auto in_fd  = _open_osfhandle (reinterpret_cast<intptr_t> (in_handle),  _O_TEXT);

    auto out_file = _fdopen (out_fd, "w");
    auto err_file = _fdopen (err_fd, "w");
    auto in_file  = _fdopen (in_fd,  "r");

    /// FILE objects are not reassignable, but Windows doesn't leave us much
    /// choice here.
    *stdout = *out_file;
    *stderr = *err_file;
    *stdin  = *in_file;

    setvbuf (stdout, NULL, _IONBF, 0);
    setvbuf (stderr, NULL, _IONBF, 0);
    setvbuf (stdin,  NULL, _IONBF, 0);

    std::ios::sync_with_stdio ();

    // Windows doesn't give an immediate newline when an application is run
    // from a console, so we provide one here for sanity.
    std::cout << "\n";
    std::cerr << "Console installed\n";
}
