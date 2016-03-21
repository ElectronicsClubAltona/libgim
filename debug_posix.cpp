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
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


///////////////////////////////////////////////////////////////////////////////
// Will return true if it is known we are under a debugger. This isn't
// designed to exhaustively check for debuggers, only to offer conveniences.
// Specifically: errors will return false.
static
bool
is_debugged (void)
{
    auto pid = fork ();
    if (pid == -1)
        return false;

    // We are the child
    if (pid == 0) {
        auto ppid = getppid ();
        int res;

        // attempt to trace our parent. this will fail if we're being debugged.
        if (ptrace (PTRACE_ATTACH, ppid, nullptr, nullptr) == 0) {
            waitpid (ppid, nullptr, 0);
            ptrace (PTRACE_DETACH, ppid, nullptr, nullptr);

            res = 0;
        } else {
            res = 1;
        }

        _exit (res);
    // We are the parent. Wait for our child to tell us the result.
    } else {
        int status;
        waitpid (pid, &status, 0);
        return WEXITSTATUS (status);
    }
}


///////////////////////////////////////////////////////////////////////////////
void
breakpoint (void)
{
    raise (SIGTRAP);
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
    if (is_debugged ())
        breakpoint ();
    else
        raise (SIGSTOP);

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
