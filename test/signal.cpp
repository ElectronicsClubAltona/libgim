#include "signal.hpp"

#include "tap.hpp"


//-----------------------------------------------------------------------------
void
test_null (util::TAP::logger &tap)
{
    tap.expect_nothrow ([] {
        util::signal<void(void)> void_signal;
        void_signal ();
    }, "void signal");
}


///////////////////////////////////////////////////////////////////////////////
void
increment_uint (unsigned int &val)
{
    ++val;
}


//-----------------------------------------------------------------------------
void
test_single (util::TAP::logger &tap)
{
    unsigned int val = 0;
    util::signal<void(unsigned int&)> void_signal;

    auto cookie = void_signal.connect (increment_uint);
    void_signal (val);

    tap.expect_eq (val, 1u, "single listener");
}


//-----------------------------------------------------------------------------
void
test_double (util::TAP::logger &tap)
{
    unsigned int val = 0;
    util::signal<void(unsigned int&)> void_signal;

    auto cookie0 = void_signal.connect (increment_uint);
    auto cookie1 = void_signal.connect (increment_uint);
    void_signal (val);

    tap.expect_eq (val, 2u, "double listener");
}


///////////////////////////////////////////////////////////////////////////////
void
test_value_signal (util::TAP::logger &tap)
{
    util::value_signal<unsigned> val;
    unsigned passed = 0;

    auto cookie = val.connect ([&] (unsigned v) { passed = v; });

    val = 42u;
    
    tap.expect_eq (passed, 42u, "value signal, passed value");

    unsigned check = val;
    tap.expect_eq (check, 42u, "value signal, read value");
}


///////////////////////////////////////////////////////////////////////////////
void
test_combiner (util::TAP::logger &tap)
{
    {
        util::signal<bool(void), util::combine::logical_and> sig;

        unsigned count = 0;
        auto cookie0 = sig.connect ([&] (void) { ++count; return true; });
        auto cookie1 = sig.connect ([&] (void) { ++count; return true; });
        auto cookie2 = sig.connect ([&] (void) { ++count; return true; });

        tap.expect (sig (), "bool signal, success");
        tap.expect_eq (count, 3u, "bool signal, success, count");
    }

    {
        util::signal<bool(void), util::combine::logical_and> sig;

        unsigned count = 0;
        auto cookie0 = sig.connect ([&] (void) { ++count; return true; });
        auto cookie1 = sig.connect ([&] (void) { ++count; return false; });
        auto cookie2 = sig.connect ([&] (void) { ++count; return true; });

        tap.expect (!sig (), "bool signal, failure");

        // ordering of signals is not guaranteed so we can't say for sure how
        // many callbacks will be triggered; it will _probably_ be in order
        // though.
        tap.expect_le (count, 3u, "bool signal, failure, count");
    }
}


///////////////////////////////////////////////////////////////////////////////
void
test_disconnect (util::TAP::logger &tap)
{
    tap.expect_nothrow ([] {
        util::signal<void(void)> sig;

        util::signal<void(void)>::cookie a = sig.connect ([&] (void) { sig.disconnect (a); });
        util::signal<void(void)>::cookie b = sig.connect ([&] (void) { sig.disconnect (b); });
        util::signal<void(void)>::cookie c = sig.connect ([&] (void) { sig.disconnect (c); });
        util::signal<void(void)>::cookie d = sig.connect ([&] (void) { sig.disconnect (d); });

        sig ();
    }, "parallel disconnect in invocation");
}


///////////////////////////////////////////////////////////////////////////////
int
main (int, char **)
{
    util::TAP::logger tap;

    test_null   (tap);
    test_single (tap);
    test_double (tap);
    test_value_signal (tap);
    test_combiner (tap);
    test_disconnect (tap);

    return tap.status ();
}
