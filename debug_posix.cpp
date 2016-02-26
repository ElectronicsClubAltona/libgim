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
 * Copyright:
 *      2011-2016, Danny Robson <danny@nerdcruft.net>
 */

#include "./debug.hpp"

#include "./log.hpp"

#include <unistd.h>
#include <sys/ptrace.h>
#include <signal.h>


///////////////////////////////////////////////////////////////////////////////
void
breakpoint (void)
{
#if defined (__x86_64) || defined (__i386)
    __asm__ ("int $3;");
#else
    raise (SIGINT);
#endif
}


///////////////////////////////////////////////////////////////////////////////
void
prepare_debugger (void)
{
    ;
}


//-----------------------------------------------------------------------------
void
await_debugger (void)
{
    if (ptrace (PTRACE_TRACEME))
        LOG_CRITICAL ("unable to wait for debugger");
}


///////////////////////////////////////////////////////////////////////////////
#include <fenv.h>
void
enable_fpe (void)
{
    feenableexcept (FE_DIVBYZERO | FE_INVALID);
}


//-----------------------------------------------------------------------------
void
disable_fpe (void)
{
    feenableexcept (0);
}


///////////////////////////////////////////////////////////////////////////////
void
force_console (void)
{
    if (!isatty (fileno (stdout))) {
        LOG_ERROR ("expected debugging under a console");
    }
}
