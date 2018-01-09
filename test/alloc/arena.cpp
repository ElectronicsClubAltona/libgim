#include "tap.hpp"

#include "alloc/arena.hpp"
#include "alloc/raw/linear.hpp"
#include "debug.hpp"


///////////////////////////////////////////////////////////////////////////////
static char g_backing[1024*1024];


//-----------------------------------------------------------------------------
struct setter {
    setter (const setter&) = delete;

    setter (bool &_target):
        target (_target)
    { target = false; }

    ~setter ()
    { target = true; }

    bool &target;
};


//-----------------------------------------------------------------------------
int
main (void)
{
    util::alloc::raw::linear alloc (std::begin (g_backing), std::end (g_backing));
    util::alloc::arena<util::alloc::raw::linear>  arena (alloc);

    util::TAP::logger tap;

    bool flag = true;

    // double check our testing object is working, because I'm tired and stupid
    {
        setter val (flag);
        CHECK (!flag);
    }
    CHECK (flag);

    // ensure manual acquire and release calls constructors and destructors
    {
        auto obj = arena.acquire<setter> (flag);
        tap.expect_eq (flag, false, "arena manual acquire calls constructor");

        arena.release (obj);
        tap.expect_eq (flag, true, "arena manual release calls destructor");
    }

    // ensure unique_ptr like objects call constructors and destructors
    {
        auto obj = arena.unique<setter> (flag);
        tap.expect_eq (flag, false, "arena unique acquire calls constructor");
    }

    tap.expect_eq (flag, true, "arena unique release calls destructor");

    return tap.status ();
}
