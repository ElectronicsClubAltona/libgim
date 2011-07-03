#include <cstdlib>

#include "../signal.hpp"
#include "../debug.hpp"

void
increment_uint (unsigned int &val)
    { ++val; }


void
test_null (void) {
    signal<void> void_signal;
    void_signal ();
}


void
test_single (void) {
    unsigned int val = 0;
    signal<void, unsigned int&> void_signal;

    void_signal.connect (increment_uint);
    void_signal (val);

    check_eq (val, 1);
}


void
test_double (void) {
    unsigned int val = 0;
    signal<void, unsigned int&> void_signal;

    void_signal.connect (increment_uint);
    void_signal.connect (increment_uint);
    void_signal (val);

    check_eq (val, 2);
}


void
test_linking_pointers (void) {
    signal<void, const char*> ptr_signal;
    ptr_signal (NULL);
}


int
main (int, char **) {
    test_null   ();
    test_single ();
    test_double ();

    return EXIT_SUCCESS;
}
