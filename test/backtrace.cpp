#include "../backtrace.hpp"
#include <iostream>

using namespace std;


int
main (int, char **) {
    cout << debug::backtrace() << endl;

    return EXIT_SUCCESS;
}
