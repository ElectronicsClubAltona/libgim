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
 * Copyright 2015-2018, Danny Robson <danny@nerdcruft.net>
 */

#ifdef __UTIL_DEBUG_IPP
#error
#endif

#define __UTIL_DEBUG_IPP

#include "backtrace.hpp"
#include "format.hpp"

#include <limits>
#include <iostream>


///////////////////////////////////////////////////////////////////////////////
namespace util::debug::detail {
    void panic [[noreturn]] (const char *msg);

    template <typename ...Args, size_t N>
    void panic [[noreturn]] (const char (&fmt)[N], const Args& ...args)
    {
        std::cerr << ::util::format::printf (fmt, args...) << ::debug::backtrace () << std::endl;
        breakpoint ();
        abort ();
    }

    void not_implemented [[noreturn]] (const char *msg);
    void unreachable [[noreturn]] (const char *msg);
}

// not_implemented/unreachable/panic must be callable from constexpr contexts.
// but they rely on functions that aren't constexpr to perform the controlled
// abort.
//
// we can work around this in the same way assert does by using a conditional
// that hides an extern function that actually does the work. as we can't
// utilise external state this has to be the message variable which will
// assume isn't ever null.
//
// to avoid warnings about a return from a noreturn function we recurse into
// ourself in the alternate case. this branch shouldn't ever be taken, but if
// it is we were planning on crashing anyway...


///////////////////////////////////////////////////////////////////////////////
constexpr void not_implemented [[noreturn]] (void)
{
    not_implemented ("operation not implemented");
}


//-----------------------------------------------------------------------------
constexpr void not_implemented [[noreturn]] (const char *msg)
{
    ! msg
    ? not_implemented (msg)
    : util::debug::detail::not_implemented (msg);
}


///////////////////////////////////////////////////////////////////////////////
constexpr void unreachable [[noreturn]] (void)
{
    unreachable ("unreachable code executed");
}


//-----------------------------------------------------------------------------
constexpr void unreachable [[noreturn]] (const char *msg)
{
    ! msg
    ? unreachable (msg)
    : util::debug::detail::unreachable (msg);
}


///////////////////////////////////////////////////////////////////////////////
constexpr void panic [[noreturn]] (const char *msg)
{
    ! msg
    ? panic (msg)
    : util::debug::detail::panic (msg);
}


//-----------------------------------------------------------------------------
template <typename ...Args, size_t N>
constexpr
void
panic [[noreturn]] (const char (&fmt)[N], const Args& ...args)
{
    util::debug::detail::panic (fmt, args...);
}
