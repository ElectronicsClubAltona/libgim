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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#include "except.hpp"

#include "../debug.hpp"


//-----------------------------------------------------------------------------
using namespace std;
using namespace net;


//-----------------------------------------------------------------------------
net::error::error (const std::string &_what):
    runtime_error (_what)
{ ; }


net::error::error (int _code):
    runtime_error (strerror (_code))
{ CHECK (_code != 0); }


std::string
net::error::code_to_string (int code) {
#ifdef __WIN32
    char message[256];
    int output = FormatMessage (0, NULL, code, 0, message, sizeof (message), NULL);
    CHECK_HARD (output != 0);

    return std::string (message);
#else
    return strerror (code);
#endif
}


void
net::error::throw_code (int code) {
#ifdef __WIN32
    throw net::error (code);
#else

    CHECK (code != 0);
    switch (code) {
#define ERROR_CODE(c)                       \
        case c:                             \
            throw net::error_code<c> ();    \
            break;

        ERROR_CODE(ECONNREFUSED);
        ERROR_CODE(ECONNRESET);
        ERROR_CODE(EINVAL);
        ERROR_CODE(ENOTCONN);

        default:
            unreachable ();
#undef ERROR_CODE
    }
#endif
}


void
net::error::try_code (int err) {
    if (err == 0)
        return;

    throw_code (err);
}


void
net::error::try_code (void)
    { try_code (last_code ()); }


void
net::error::throw_code (void)
    { throw_code (last_code ()); }


int
net::error::last_code (void) {
#ifdef __WIN32
    return WSAGetLastError ();
#else
    return errno;
#endif
}


template <int CODE>
net::error_code<CODE>::error_code (void):
    net::error (CODE)
{ ; }


template <int CODE>
int
net::error_code<CODE>::code (void) const 
    { return CODE; }
