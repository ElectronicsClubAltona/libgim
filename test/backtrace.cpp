#include "backtrace.hpp"
#include "tap.hpp"
#include "stream.hpp"

#include <iostream>
#include <cstdlib>


///////////////////////////////////////////////////////////////////////////////
int
main (int, char **) {
    util::TAP::logger tap;

    debug::backtrace ();
    tap.noop ();

    return EXIT_SUCCESS;
}
