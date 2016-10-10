#include "./tap.hpp"

#include "./exe.hpp"

#include <iostream>


///////////////////////////////////////////////////////////////////////////////
int
main (int, char**)
{
    util::TAP::logger tap;
    tap.expect_eq (util::image_path ().stem (), "exe", "identify executable path");
    return tap.status ();
}
