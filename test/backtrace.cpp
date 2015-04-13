#include "backtrace.hpp"
#include "tap.hpp"
#include "stream.hpp"

#include <iostream>
#include <cstdlib>

using namespace std;


int
main (int, char **) {
    util::TAP::logger tap;

    util::stream::null out;
    out << debug::backtrace() << endl;
    tap.noop ();

    return EXIT_SUCCESS;
}
