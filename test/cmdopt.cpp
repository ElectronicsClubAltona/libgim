#include "cmdopt.hpp"

#include "debug.hpp"
#include "tap.hpp"
#include "types.hpp"
#include "maths.hpp"


//-----------------------------------------------------------------------------
// Check that null options don't strhrow anything
void
test_null (void) {
    util::cmdopt::parser p;
    auto n = p.add<util::cmdopt::option::null> ('n', "null", "testing null option");

    static const char *argv1[] = { "./foo", "-n", "foo" };
    p.scan (elems (argv1), argv1);

    static const char *argv2[] = { "./foo", "--null", "foo" };
    p.scan (elems (argv2), argv2);
}


//-----------------------------------------------------------------------------
// Check if presence options can be used successfully
void
test_present (void) {
    util::cmdopt::parser p;

    bool is_present;
    p.add<util::cmdopt::option::present> ('p', "present", "option is present", is_present);

    // Short option form
    static const char *argv1[] = { "./foo", "-p" };
    is_present = false;
    p.scan (elems (argv1), argv1);
    CHECK (is_present);

    // Long option form
    static const char *argv2[] = { "./foo", "--present" };
    is_present = false;
    p.scan (elems (argv2), argv2);
    CHECK (is_present);

    // Check that value is reset from true if not present
    static const char *argv3[] = { "./foo" };
    is_present = true;
    p.scan (elems (argv3), argv3);
    CHECK (!is_present);
}


//-----------------------------------------------------------------------------
// Check all forms of boolean inputs
void
test_bool (void) {
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
        CHECK_EQ (value, true);
    }

    for (auto i: negative) {
        argv[2] = i;
        p.scan (argv.size (), argv.data ());
        CHECK_EQ (value, false);
    }

    // Check that invalid forms of boolean all throw exceptions
    const char* invalid[] = { "foo", "null" };

    for (auto i: invalid) {
        argv[2] = i;
        CHECK_THROWS (
            util::cmdopt::invalid_value,
            p.scan (argv.size (), argv.data ())
        );
    }
}


//-----------------------------------------------------------------------------
template<typename T>
void
test_numeric (void) {
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

        out_short << values[i];
        str_short = out_short.str ();
        argv_short[2] = str_short.c_str ();

        value = 2;
        p.scan (elems (argv_short), argv_short);
        CHECK (value == values[i]);

        out_long << "--type=" << values[i];
        str_long = out_long.str ();
        argv_long[1] = str_long.c_str ();

        value = 2;
        p.scan (elems (argv_long), argv_long);
        CHECK (value == values[i]);
    }
}


//-----------------------------------------------------------------------------
void
test_bytes (void) {
    util::cmdopt::parser p;
    size_t size;

    p.add<util::cmdopt::option::bytes> ('b', "bytes", "testing bytes", size);

    static const struct {
        const char *str;
        size_t      val;
    } commands[] = {
        { "1",  1 },
        { "1k", pow (1024, 1) },
        { "1M", pow (1024, 2) },
        { "1G", pow (1024, 3) }
    };

    const char *argv[] = {
        "./foo",
        "-b",
        NULL
    };

    for (auto &i: commands) {
        argv[2] = i.str;
        p.scan (elems (argv), argv);

        CHECK_EQ (i.val, size);
    };
}


//-----------------------------------------------------------------------------
void
test_required (void) {
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

    CHECK_NOTHROW (p.scan (elems (argv), argv));
    CHECK_THROWS  (util::cmdopt::invalid_required, p.scan (1, argv));
}


//-----------------------------------------------------------------------------
int
main (int, char **) {
    util::TAP::logger tap;
    tap.todo ("convert to TAP");

    test_null ();
    test_present ();
    test_bool ();
    test_numeric< int16_t> ();
    test_numeric< int32_t> ();
    test_numeric< int64_t> ();
    test_numeric<uint16_t> ();
    test_numeric<uint32_t> ();
    test_numeric<uint64_t> ();
    test_bytes ();
    test_required ();
}
