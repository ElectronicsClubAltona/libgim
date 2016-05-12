#include "cmdopt.hpp"

#include "debug.hpp"
#include "tap.hpp"
#include "types.hpp"
#include "maths.hpp"

#include <array>


///////////////////////////////////////////////////////////////////////////////
// Check that null options don't strhrow anything
static
void
test_null (util::TAP::logger &tap)
{
    util::cmdopt::parser p;
    p.add<util::cmdopt::option::null> ('n', "null", "testing null option");

    static const char *argv1[] = { "./foo", "-n", "foo" };
    tap.expect_nothrow ([&] () {
        p.scan (elems (argv1), argv1);
    }, "nothrow null short form");

    static const char *argv2[] = { "./foo", "--null", "foo" };
    tap.expect_nothrow ([&] () {
        p.scan (elems (argv2), argv2);
    }, "nothrow null long form");
}


///////////////////////////////////////////////////////////////////////////////
// Check if presence options can be used successfully
static
void
test_present (util::TAP::logger &tap)
{
    util::cmdopt::parser p;
    bool is_present;

    p.add<util::cmdopt::option::present> ('p', "present", "option is present", is_present);

    // Short option form
    {
        static const char *argv1[] = { "./foo", "-p" };
        is_present = false;
        p.scan (elems (argv1), argv1);
        tap.expect (is_present, "presence short form");
    }

    // Long option form
    {
        static const char *argv2[] = { "./foo", "--present" };
        is_present = false;
        p.scan (elems (argv2), argv2);
        tap.expect (is_present, "presence long form");
    }

    // Check that value is reset from true if not present
    {
        static const char *argv3[] = { "./foo" };
        is_present = true;
        p.scan (elems (argv3), argv3);
        tap.expect (!is_present, "presence null");
    }
}


///////////////////////////////////////////////////////////////////////////////
// Check all forms of boolean inputs
static
void
test_bool (util::TAP::logger &tap)
{
    util::cmdopt::parser p;
    bool value = false;

    p.add<util::cmdopt::option::value<bool>> ('b', "bool", "testing boolean actions", value);

    // List all legal forms of positive or negative boolean values
    std::array<const char*, 3> argv { "./foo", "-b", NULL };
    static const char *positive[] = { "1", "true", "yes" };
    static const char *negative[] = { "0", "false", "no" };

    // For each boolean value, ensure that it returns as expected
    for (auto i: positive) {
        argv[2] = i;
        p.scan (argv.size (), argv.data ());
        tap.expect_eq (value, true, i, "read bool, %s", i);
    }

    for (auto i: negative) {
        argv[2] = i;
        p.scan (argv.size (), argv.data ());
        tap.expect_eq (value, false, i, "read bool, %s", i);
    }

    // Check that invalid forms of boolean all throw exceptions
    const char* invalid[] = { "foo", "null" };

    for (auto i: invalid) {
        argv[2] = i;
        tap.expect_throw<util::cmdopt::invalid_value> ([&] () {
            p.scan (argv.size (), argv.data ());
        }, "invalid bool, %s", i);
    }
}


///////////////////////////////////////////////////////////////////////////////
template<typename T>
static
void
test_numeric (util::TAP::logger &tap)
{
    util::cmdopt::parser p;

    T value;
    p.add<util::cmdopt::option::value<T>> ('t', "type", "testing type option", value);

    T values[] = {
        // TODO: Enable minimum value testing. Currently disabled as
        // a negative numerical value looks just like a proceeding
        // option.
        //std::numeric_limits<T>::min(),

        std::numeric_limits<T>::max (),
        0
    };

    const char *argv_short[] = { "./foo", "-t", NULL };
    const char * argv_long[] = { "./foo", NULL };

    for(size_t i = 0; i < elems (values); ++i) {
        std::ostringstream out_short, out_long;
        std::string        str_short, str_long;

        // construct short form arguments
        out_short << values[i];
        str_short = out_short.str ();
        argv_short[2] = str_short.c_str ();

        // check short form reading
        value = 2;
        p.scan (elems (argv_short), argv_short);
        tap.expect_eq (value, values[i], "equality, short form");

        // construct long form arguments
        out_long << "--type=" << values[i];
        str_long = out_long.str ();
        argv_long[1] = str_long.c_str ();

        // check long form reading
        value = 2;
        p.scan (elems (argv_long), argv_long);
        tap.expect_eq (value, values[i], "equality, long form");
    }
}


///////////////////////////////////////////////////////////////////////////////
static
void
test_bytes (util::TAP::logger &tap)
{
    util::cmdopt::parser p;
    size_t size;

    p.add<util::cmdopt::option::bytes> ('b', "bytes", "testing bytes", size);

    static const struct {
        const char *str;
        size_t      val;
    } commands[] = {
        { "1",  1 },
        { "1k", util::pow (1024u, 1u) },
        { "1M", util::pow (1024u, 2u) },
        { "1G", util::pow (1024u, 3u) }
    };

    const char *argv[] = {
        "./foo",
        "-b",
        NULL
    };

    for (auto &i: commands) {
        argv[2] = i.str;
        p.scan (elems (argv), argv);

        tap.expect_eq (i.val, size, "bytes, %s", i.str);
    };
}


///////////////////////////////////////////////////////////////////////////////
static
void
test_required (util::TAP::logger &tap)
{
    util::cmdopt::parser p;
    p.add<util::cmdopt::option::null> (
        'n',
        "null",
        "null testing"
    ).required (true);

    static const char *argv[] = {
        "./cpptest",
        "-n",
        "value"
    };

    tap.expect_nothrow ([&] () {
        p.scan (elems (argv), argv);
    }, "required option, success");

    tap.expect_throw<util::cmdopt::invalid_required> ([&] () {
        p.scan (1, argv);
    }, "required option, failure");
}


///////////////////////////////////////////////////////////////////////////////
static
void
test_positional (util::TAP::logger &tap)
{
    unsigned value = 0xdead;
    constexpr unsigned expected = 0xbeef;

    util::cmdopt::parser p;
    p.append<util::cmdopt::option::value<unsigned>> ("unsigned test", value);

    static const char *argv[] = {
        "./cpptest",
        "--",
        "48879",
    };

    tap.expect_nothrow ([&] {
        p.scan (elems (argv), argv);
    }, "positional, nothrow");

    tap.expect_eq (value, expected, "positiona, value success");
}


///////////////////////////////////////////////////////////////////////////////
int
main (int, char **) {
    util::TAP::logger tap;

    test_null (tap);
    test_present (tap);
    test_bool (tap);
    test_numeric< int16_t> (tap);
    test_numeric< int32_t> (tap);
    test_numeric< int64_t> (tap);
    test_numeric<uint16_t> (tap);
    test_numeric<uint32_t> (tap);
    test_numeric<uint64_t> (tap);
    test_bytes (tap);
    test_required (tap);
    test_positional (tap);

    return tap.status ();
}
