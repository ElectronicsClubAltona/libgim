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
 * Copyright 2010-2016 Danny Robson <danny@nerdcruft.net>
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
void
util::debug::init (void)
{
    force_console ();
    prepare_debugger ();

    if (getenv ("DEBUG_WAIT"))
        await_debugger ();
}
