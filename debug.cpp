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

#include "debug.hpp"
#include "backtrace.hpp"
#include "log.hpp"
#include "platform.hpp"

#include <cstdlib>
#include <iostream>

using namespace std;


//------------------------------------------------------------------------------
void
panic (const std::string& what) {
    breakpoint ();
    cerr << "PANIC: " << what << "\n" << debug::backtrace () << endl;
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
    static const char *MSG = "Function not implemented";
    not_implemented (MSG);
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
