#include "../backtrace.hpp"
#include <iostream>

using namespace std;


int
main (int argc, char **argv) {
    cout << debug::backtrace() << endl;

    return EXIT_SUCCESS;
}
