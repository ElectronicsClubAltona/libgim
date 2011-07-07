/*
 * This file is part of libgim.
 *
 * Waif is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * Waif is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2010 Danny Robson <danny@blubinc.net>
 */

#include "debug.hpp"
#include "backtrace.hpp"

#include <cstdlib>
#include <iostream>

using namespace std;


void
panic (const std::string& what) {
    breakpoint ();
    cerr << "PANIC: " << what << "\n" << debug::backtrace () << endl;
    abort ();
}


void
panic (void)
    { panic ("NFI"); }


void
breakpoint (void) {
    if (getenv ("DEBUG")) {
#if defined (__x86_64) || defined (__i386)
        __asm__ ("int $3;");
#else
        raise (SIGINT);
#endif
    }
}


void
not_implemented (void)
    { panic ("Function not implemented"); }


void
unreachable (void) {
    panic ("Unreachable code executed");
}


void
unreachable (const std::string& what) {
    panic (" Unreachable code executed: " + what); 
}
