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

    // Ensure numeric_limits has been specialised. Unknown types are default initialised, so check if we get non-zero for maximum.
    tap.expect_eq (std::numeric_limits<decltype(fortytwo)>::max (),
                   std::numeric_limits<decltype(fortytwo)::value_type>::max (),
                   "numeric_limits has been specialised");

    return tap.status ();
}
