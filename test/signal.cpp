#include <cstdlib>

#include "signal.hpp"
#include "debug.hpp"
#include "raii.hpp"


//-----------------------------------------------------------------------------
void
test_null (void)
{
    util::signal<void(void)> void_signal;
    void_signal ();
}


///////////////////////////////////////////////////////////////////////////////
void
increment_uint (unsigned int &val)
{
    ++val;
}


//-----------------------------------------------------------------------------
void
test_single (void)
{
    unsigned int val = 0;
    util::signal<void(unsigned int&)> void_signal;

    auto raii = void_signal.connect (increment_uint);
    void_signal (val);

    CHECK_EQ (val, 1);
}


//-----------------------------------------------------------------------------
void
test_double (void)
{
    unsigned int val = 0;
    util::signal<void(unsigned int&)> void_signal;

    auto raii = void_signal.connect (increment_uint);
    auto raii = void_signal.connect (increment_uint);
    void_signal (val);

    CHECK_EQ (val, 2);
}


///////////////////////////////////////////////////////////////////////////////
void
test_linking_pointers (void)
{
    util::signal<void(const char*)> ptr_signal;
    ptr_signal (nullptr);
}


///////////////////////////////////////////////////////////////////////////////
void
test_value_signal (void)
{
    util::value_signal<unsigned> val;
    auto raii = val.connect ([] (unsigned v) { CHECK_EQ (v, 42); });
    val = 42u;

    unsigned check = val;
    CHECK_EQ (check, 42);
}


///////////////////////////////////////////////////////////////////////////////
int
main (int, char **)
{
    test_null   ();
    test_single ();
    test_double ();
    test_value_signal ();

    return EXIT_SUCCESS;
}
