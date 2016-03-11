#include "backtrace.hpp"
#include "tap.hpp"
#include "stream.hpp"

#include <iostream>
#include <cstdlib>


///////////////////////////////////////////////////////////////////////////////
int
main (int, char **) {
    util::TAP::logger tap;

    util::stream::null out;
    out << debug::backtrace() << std::endl;
    tap.noop ();

    return EXIT_SUCCESS;
}
