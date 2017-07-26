#include "parse.hpp"
#include "tap.hpp"


///////////////////////////////////////////////////////////////////////////////
int
main (void)
{
    util::TAP::logger tap;

    tap.expect_eq (util::parse<long> ("1"), 1L, "parsing long '1'");
    tap.expect_throw<std::invalid_argument> ([] () { util::parse<long> ("a"); }, "parsing long 'a'");

    tap.expect_eq (util::parse<float> ("1"), 1.f, "parsing float '1'");
    tap.expect_throw<std::invalid_argument> ([] () { util::parse<float> ("a"); }, "parsing float 'a'");

    return tap.status ();
}