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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#include "except.hpp"

#include "../debug.hpp"
#include "../types/casts.hpp"


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


//-----------------------------------------------------------------------------
std::string
net::error::code_to_string (int code) {
#ifdef __WIN32
    char message[256];

    // It should be fine to signcast the code here as Windows guarantees all
    // error messages are positive but appears to use int for compatibility
    DWORD output = FormatMessage (0, NULL, sign_cast<unsigned> (code), 0, message, sizeof (message), NULL);
    CHECK_NEQ (output, 0);

    return std::string (message);
#else
    return strerror (code);
#endif
}


//-----------------------------------------------------------------------------
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
net::error::throw_code (void)
    { throw_code (last_code ()); }


//-----------------------------------------------------------------------------
void
net::error::try_code (int err) {
    if (err == 0)
        return;

    throw_code (err);
}


void
net::error::try_code (void)
    { try_code (last_code ()); }


//-----------------------------------------------------------------------------
int
net::error::last_code (void) {
#ifdef __WIN32
    return WSAGetLastError ();
#else
    return errno;
#endif
}


//-----------------------------------------------------------------------------
template <int CODE>
net::error_code<CODE>::error_code (void):
    net::error (CODE)
{ ; }


template <int CODE>
int
net::error_code<CODE>::code (void) const 
    { return CODE; }
