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
 * Copyright 2010 Danny Robson <danny@nerdcruft.net>
 */

#include "backtrace.hpp"
#include "debug.hpp"
#include "log.hpp"
#include "platform.hpp"

#include <cstdlib>
#include <iostream>

using namespace std;


//------------------------------------------------------------------------------
void
panic (const std::string& what) {
    cerr << "PANIC: " << what << "\n" << debug::backtrace () << endl;
    breakpoint ();
    abort ();
}


void
panic (void)
    { panic ("NFI"); }


//------------------------------------------------------------------------------
#if defined(PLATFORM_WIN32)
#include <windows.h>
void
breakpoint (void) {
    DebugBreak ();
}
#else
void
breakpoint (void) {
#if defined (__x86_64) || defined (__i386)
    __asm__ ("int $3;");
#else
    raise (SIGINT);
#endif
}
#endif


//------------------------------------------------------------------------------
void
not_implemented (void) {
    not_implemented ("Function not implemented");
}


void
not_implemented (const char *msg)
    { panic (msg); }


//------------------------------------------------------------------------------
void
unreachable (void) {
    panic ("Unreachable code executed");
}


void
unreachable (const std::string& what) {
    panic (" Unreachable code executed: " + what); 
}


//------------------------------------------------------------------------------
void
unusual (void) {
    panic ("Unusual code path found.");
}


//------------------------------------------------------------------------------
#if defined(PLATFORM_LINUX)
#include <fenv.h>
void
enable_fpe (void) {
    feenableexcept (FE_DIVBYZERO | FE_INVALID);
}


void
disable_fpe (void) {
    feenableexcept (0);
}
#else
// contropfp is not defined supposedly due to a regression in GCC include behaviours.

void
enable_fpe (void) {
    LOG_WARN ("Non-Linux exception code is broken under current GCC.");
//    _clearfp();
//    unsigned int ignored;
//    _controlfp_s (&ignored, _MCW_EM, _MCW_EM);
}


void
disable_fpe (void) {
    LOG_WARN ("Non-Linux exception code is broken under current GCC.");
//    _clearfp();
//    unsigned int ignored;
//    _controlfp_s (&ignored, 0, _MCW_EM);
}
#endif


//------------------------------------------------------------------------------
#if defined(PLATFORM_WIN32)
#include <io.h>
#include <fcntl.h>

void
force_console (void) {
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


//------------------------------------------------------------------------------
void
debug::init (void) {
#if defined(PLATFORM_WIN32)
    force_console ();

    if (nullptr == LoadLibrary("exchndl.dll")) {
        std::cerr << GetLastError () << "\n";
        LOG_WARNING("Emergency debugger not loaded");
    }
#endif
}
