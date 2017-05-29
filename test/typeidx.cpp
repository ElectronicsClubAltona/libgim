#include "tap.hpp"

#include "./typeidx.hpp"


///////////////////////////////////////////////////////////////////////////////
int
main (int, char**)
{
    util::TAP::logger tap;

    tap.expect_eq (util::typeidx<int> (), util::typeidx<int> (), "equality for int");
    tap.expect_eq (util::typeidx<float> (), util::typeidx<float> (), "equality for float");
    tap.expect_neq (util::typeidx<int> (), util::typeidx<float> (), "inequality for int/float");

    return tap.status ();
}