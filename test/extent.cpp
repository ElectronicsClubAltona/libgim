#include "extent.hpp"
#include "tap.hpp"

int
main (void)
{
    util::TAP::logger tap;

    {
        util::extent2u hi { 8, 4 };
        util::extent2u lo { 6, 2 };
        tap.expect_eq (lo, hi.contracted (2), "extent scalar contraction by value");
    }

    return tap.status ();
}
