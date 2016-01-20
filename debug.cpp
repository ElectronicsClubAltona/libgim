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

#include "backtrace.hpp"
#include "debug.hpp"
#include "log.hpp"
#include "platform.hpp"

#include <cstdlib>
#include <iostream>

using namespace util::debug;


////////////////////////////////////////////////////////////////////////////////
void
detail::panic (const char *msg)
{
    std::cerr << "PANIC: " << msg << "\n" << ::debug::backtrace () << std::endl;
    breakpoint ();
    abort ();
}


////////////////////////////////////////////////////////////////////////////////
#if defined(PLATFORM_WIN32)
#include <windows.h>
void
breakpoint (void)
{
    DebugBreak ();
}
#else
void
breakpoint (void)
{
#if defined (__x86_64) || defined (__i386)
    __asm__ ("int $3;");
#else
    raise (SIGINT);
#endif
}
#endif


//-----------------------------------------------------------------------------
void
detail::not_implemented (const char *msg)
{
    panic (msg);
}


//-----------------------------------------------------------------------------
void
detail::unreachable (const char *msg)
{
    panic (msg);
}


////////////////////////////////////////////////////////////////////////////////
void
warn (void)
{
    warn ("Unusual code path found.");
}


//-----------------------------------------------------------------------------
void
warn (const std::string &msg)
{
    warn (msg.c_str ());
}


//-----------------------------------------------------------------------------
void
warn (const char *msg)
{
    LOG_WARN (msg);
}


////////////////////////////////////////////////////////////////////////////////
#if defined(PLATFORM_LINUX)
#include <fenv.h>
void
enable_fpe (void)
{
    feenableexcept (FE_DIVBYZERO | FE_INVALID);
}


//-----------------------------------------------------------------------------
void
disable_fpe (void)
{
    feenableexcept (0);
}
#else
// contropfp is not defined supposedly due to a regression in GCC include behaviours.


//-----------------------------------------------------------------------------
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
#endif


////////////////////////////////////////////////////////////////////////////////
#if defined(PLATFORM_WIN32)
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

    ios::sync_with_stdio ();

    // Windows doesn't give an immediate newline when an application is run
    // from a console, so we provide one here for sanity.
    std::cout << "\n";
    std::cerr << "Console installed\n";
}
#endif


////////////////////////////////////////////////////////////////////////////////
void
util::debug::init (void)
{
#if defined(PLATFORM_WIN32)
    force_console ();

    if (nullptr == LoadLibrary("exchndl.dll")) {
        std::cerr << GetLastError () << "\n";
        LOG_WARNING("Emergency debugger not loaded");
    }
#endif
}
