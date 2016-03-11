#include <cstdlib>

#include <iostream>

#include "coord/iostream.hpp"
#include "extent.hpp"
#include "region.hpp"

int
main (int argc, char **argv)
{
    (void)argc;
    (void)argv;

    util::point2u a { 0, 0 };
    util::point2u b { 1, 1 };
    util::region2u v { a, b };
    std::cerr << v << '\n';

    return EXIT_SUCCESS;
}
