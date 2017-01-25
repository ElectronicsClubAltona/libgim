#include "./tap.hpp"

#include "./exe.hpp"

#include <iostream>


///////////////////////////////////////////////////////////////////////////////
int
main (int, char **argv)
{
    util::TAP::logger tap;

    auto path = util::image_path ();
    tap.expect_eq (path, argv[0], "identify executable path");
    return tap.status ();
}
