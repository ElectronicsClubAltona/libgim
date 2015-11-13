#include "signal.hpp"

#include "debug.hpp"
#include "raii.hpp"
#include "tap.hpp"


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

    CHECK_EQ (val, 1u);
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

    CHECK_EQ (val, 2u);
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
    auto raii = val.connect ([] (unsigned v) { CHECK_EQ (v, 42u); });
    val = 42u;

    unsigned check = val;
    CHECK_EQ (check, 42u);
}


///////////////////////////////////////////////////////////////////////////////
void
test_combiner (void)
{
    {
        util::signal<bool(void), util::combine::logical_and> sig;

        unsigned count = 0;
        auto raii = sig.connect ([&] (void) { ++count; return true; });
        auto raii = sig.connect ([&] (void) { ++count; return true; });
        auto raii = sig.connect ([&] (void) { ++count; return true; });

        CHECK (sig ());
        CHECK_EQ (count, 3u);
    }

    {
        util::signal<bool(void), util::combine::logical_and> sig;

        unsigned count = 0;
        auto raii = sig.connect ([&] (void) { ++count; return true; });
        auto raii = sig.connect ([&] (void) { ++count; return false; });
        auto raii = sig.connect ([&] (void) { ++count; return true; });

        CHECK (!sig ());
        CHECK_EQ (count, 2u);
    }
}


///////////////////////////////////////////////////////////////////////////////
void
test_disconnect (void)
{
    util::signal<void(void)> sig;

    util::signal<void(void)>::cookie a = sig.connect ([&] (void) { sig.disconnect (a); });
    util::signal<void(void)>::cookie b = sig.connect ([&] (void) { sig.disconnect (b); });
    util::signal<void(void)>::cookie c = sig.connect ([&] (void) { sig.disconnect (c); });
    util::signal<void(void)>::cookie d = sig.connect ([&] (void) { sig.disconnect (d); });

    sig ();
}


///////////////////////////////////////////////////////////////////////////////
int
main (int, char **)
{
    test_null   ();
    test_single ();
    test_double ();
    test_value_signal ();
    test_combiner ();
    test_disconnect ();

    util::TAP::logger tap;
    tap.skip ("convert to TAP");
}
