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
