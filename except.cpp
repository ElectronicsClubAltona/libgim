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

#include "except.hpp"
#include "debug.hpp"

#include <cstring>
#include <cerrno>

using namespace std;


/// Construct an errno_error from a given error value. The error value MUST signal an error at
/// construction time.
errno_error::errno_error (int _errno):
    runtime_error (strerror (_errno)),
    id            (_errno)
{
    check_hard (_errno != 0);
}


/// Construct an errno_error from the current value of errno. errno MUST signal an error at
/// construction time.
errno_error::errno_error ():
    runtime_error (strerror (errno)),
    id            (errno)
{
    check_hard (errno != 0);
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
    check_hard (code != 0);
    throw errno_error (code);
}
