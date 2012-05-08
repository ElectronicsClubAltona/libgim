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

#include "time.hpp"

#include "debug.hpp"
#include "platform.hpp"
#include "types.hpp"


using namespace util;

#ifdef PLATFORM_WIN32

#include <windows.h>

uint64_t
util::nanoseconds (void) {
    LARGE_INTEGER freq, count;
    QueryPerformanceFrequency (&freq);
    QueryPerformanceCounter   (&count); 

    return ((double)count.QuadPart / freq.QuadPart) * 1000000000UL;
}


void
util::sleep (uint64_t ns) {
    static const uint64_t ns_in_ms = 1000000;
    Sleep (ns / ns_in_ms);
}

#else

#include <ctime>

uint64_t
util::nanoseconds (void) {
    struct timespec t;
    clock_gettime (CLOCK_REALTIME, &t);

    check_soft (t.tv_sec  > 0);
    check_soft (t.tv_nsec > 0);

    return static_cast<uint64_t> (t.tv_sec) * 1000000000ULL + static_cast<uint64_t> (t.tv_nsec);
}


void
util::sleep (uint64_t ns) {
    struct timespec req, rem;

    req.tv_sec  = sign_cast<time_t> (ns / 1000000000UL);
    req.tv_nsec = sign_cast<long>   (ns % 1000000000UL);

    while (nanosleep (&req, &rem)) {
        req = rem;
    }
}

#endif



