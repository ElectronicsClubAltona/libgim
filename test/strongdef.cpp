#include "tap.hpp"
#include "strongdef.hpp"

using util::strongdef;


int
main (void)
{
    util::TAP::logger tap;
    
    // These tests are less about functional testing, and more about link testing.
    strongdef<unsigned> fortytwo (42u);
    tap.expect_eq (fortytwo.data, 42u, "raw data equality");
    tap.expect_eq (fortytwo,      42u, "passthrough equality");

    return tap.status ();
}
