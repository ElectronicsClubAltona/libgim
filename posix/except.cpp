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
 * Copyright 2010, 2017,
 *     Danny Robson <danny@nerdcruft.net>
 */

#include "except.hpp"

#include "../debug.hpp"

#include <cstring>

using util::posix::error;


///////////////////////////////////////////////////////////////////////////////
/// Construct an exception object from a given error value.
///
/// The error value MUST be an error at construction time.
error::error (int _code):
    std::runtime_error (::strerror (_code)),
    m_code (_code)
{
    CHECK_NEQ (_code, 0);
}


///----------------------------------------------------------------------------
/// Construct an exception object from the current value of errno.
///
/// errno MUST signal an error at construction time.
error::error ():
    error (last_code ())
{
    CHECK_NEQ (m_code, 0);
}


///////////////////////////////////////////////////////////////////////////////
int
error::last_code (void)
{
    return errno;
}


//-----------------------------------------------------------------------------
int
error::code (void) const
{
    return m_code;
}


///////////////////////////////////////////////////////////////////////////////
/// Throw an exception object if errno currently signals an error.
void
error::try_code (void)
{
    try_code (last_code ());
}


///----------------------------------------------------------------------------
/// Throw an exception object if 'code' represents an error.
void
error::try_code (int code)
{
    if (__builtin_expect (code != 0, false))
        throw error (code);
}


///----------------------------------------------------------------------------
void
error::throw_code (void)
{
    throw_code (last_code ());
}


///----------------------------------------------------------------------------
void
error::throw_code (int code)
{
    CHECK_NEQ (code, 0);
    throw error (code);
}
