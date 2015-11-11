#include "tap.hpp"
#include "memory/buffer/paged.hpp"
#include "debug.hpp"
#include "except.hpp"

#include <signal.h>
#include <setjmp.h>

///////////////////////////////////////////////////////////////////////////////
sigjmp_buf fault_jmp;


template <typename T>
bool
has_fault (const volatile T* addr)
{
    if (sigsetjmp (fault_jmp, 1) == 0) {
        *addr;
        return false;
    } else {
        return true;
    }
}


//-----------------------------------------------------------------------------
static bool  fault_seen;
static void *fault_address;

void
segv_handler (int num, siginfo_t *info, void *cookie)
{
    CHECK_EQ (num, SIGSEGV);

    (void)num;
    (void)cookie;

    fault_seen    = true;
    fault_address = info->si_addr;

    siglongjmp (fault_jmp, SIGSEGV);
}



///////////////////////////////////////////////////////////////////////////////
int
main (void)
{
    util::TAP::logger tap;

    // setup a trap to record SEGV events
    struct sigaction newhandler {};
    newhandler.sa_sigaction = segv_handler;
    newhandler.sa_flags = SA_SIGINFO;

    auto err = sigaction (SIGSEGV, &newhandler, nullptr);
    if (err)
        util::errno_error::throw_code ();

    // initialise a partially unmapped buffer. the tests assume that the
    // window is substantially less than half the capacity (so that probing
    // the centre doesn't trigger a mapping overlapping the end).
    constexpr size_t CAPACITY = 16 * 1024 * 1024;
    constexpr size_t WINDOW   = 1024 * 1024;

    util::memory::buffer::paged buffer (CAPACITY, WINDOW);

    typedef decltype(buffer)::value_type value_type;
    const value_type *first = buffer.begin ();
    const value_type *last = buffer.end () - 1;
    const value_type *centre = buffer.begin () + buffer.capacity () / 2;
    const value_type *window = centre + buffer.window () - 1;

    // ensure correct initial mappings
    tap.expect (!has_fault (first), "first is initially valid");
    tap.expect ( has_fault (last),  "last is intially invalid");

    // allocate half the buffer and check mappings
    buffer.access (const_cast<value_type*> (centre));

    tap.expect (!has_fault (first),  "first remains valid after commit");
    tap.expect (!has_fault (centre), "centre is valid after partial commit");
    tap.expect (!has_fault (window), "centre window is valid after partial commit");
    tap.expect ( has_fault (last),   "last is invalid after partial commit");

    // allocate the entire buffer and check the last address
    buffer.access (const_cast<value_type*> (last));

    tap.expect (!has_fault (last), "last is valid after total commit");

    // unmap the buffer and check centre and last are invalid
    buffer.access (const_cast<value_type*> (first));

    tap.expect (!has_fault (first),  "first value remains valid after release");
    tap.expect ( has_fault (centre), "centre is invalid after release");
    tap.expect ( has_fault (last),   "last is invalid after release");

    return tap.status ();
}
