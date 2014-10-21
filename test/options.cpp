#include "options.hpp"

#include "debug.hpp"
#include "types.hpp"

#include <cassert>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <limits>
#include <sstream>
#include <cstdint>
#include <memory>


using namespace std;
using namespace util;


// Check that null options don't throw anything
void
test_null_opt (void) {
    unique_ptr<processor> p(new processor());

    p->add_option(make_unique<nulloption> ('n', "null", "testing null option"));

    static const char *argv1[] = { "./foo", "-n", "foo" };
    p->parse_args(elems(argv1), argv1);

    static const char *argv2[] = { "./foo", "--null", "foo" };
    p->parse_args(elems(argv2), argv2);
}




// Check if presence options can be used successfully
void
test_present_opt (void) {
    unique_ptr<processor> p(new processor());
    bool is_present;
    p->add_option(make_unique<presentoption> ('p', "present", "option is present", &is_present));

    // Short option form
    static const char *argv1[] = { "./foo", "-p" };
    is_present = false;
    p->parse_args(elems(argv1), argv1);
    CHECK (is_present);

    // Long option form
    static const char *argv2[] = { "./foo", "--present" };
    is_present = false;
    p->parse_args(elems(argv2), argv2);
    CHECK (is_present);

    // Check that value is reset from true if not present
    static const char *argv3[] = { "./foo" };
    is_present = true;
    p->parse_args(elems(argv3), argv3);
    CHECK (!is_present);
}


// Check all forms of boolean inputs
void
test_bool_opt (void) {
    unique_ptr<processor> p(new processor());
    bool value = false;

    p->add_option (make_unique<valueoption<bool>> ('b', "bool", "testing boolean actions", &value));

    // List all legal forms of positive or negative boolean values
    std::array<const char*, 3> argv { "./foo", "-b", NULL };
    static const char *positive[] = { "1", "true", "yes" };
    static const char *negative[] = { "0", "false", "no" };

    // For each boolean value, ensure that it returns as expected
    for (auto i: positive) {
        static size_t count;
        std::cerr << "iter " << count++ << '\n';
        argv[2] = i;
        p->parse_args (argv.size (), argv.data ());
        CHECK (value == true);
    }

    for (auto i: negative) {
        argv[2] = i;
        p->parse_args (argv.size (), argv.data ());
        CHECK (value == false);
    }

    // Check that invalid forms of boolean all throw exceptions
    const char* invalid[] = { "foo", "y", "null" };

    for (auto i: invalid) {
        argv[2] = i;
        CHECK_THROWS (
            std::domain_error,
            p->parse_args (argv.size (), argv.data ())
        );
    }
}


template<typename T>
void
test_numeric_opt (void) {
    unique_ptr<processor> p(new processor ());
    T value;
    p->add_option (make_unique<valueoption<T>> ('t', "type", "testing type option", &value));

    T values[] = {
        // TODO: Enable minimum value testing. Currently disabled as
        // a negative numerical value looks just like a proceeding
        // option.
        //numeric_limits<T>::min(),

        numeric_limits<T>::max (),
        0
    };

    const char * argv_short[] = { "./foo", "-t", NULL };
    const char *  argv_long[] = { "./foo", NULL };

    for(size_t i = 0; i < elems (values); ++i) {
        ostringstream out_short, out_long;
        string        str_short, str_long;

        out_short << values[i];
        str_short = out_short.str ();
        argv_short[2] = str_short.c_str ();

        value = 2;
        p->parse_args (elems (argv_short), argv_short);
        CHECK (value == values[i]);

        out_long << "--type=" << values[i];
        str_long = out_long.str ();
        argv_long[1] = str_long.c_str ();

        value = 2;
        p->parse_args (elems (argv_long), argv_long);
        CHECK (value == values[i]);
    }
}


void
test_bytes_opt(void) {
    unique_ptr<processor> p(new processor ());

    static const struct {
        const char                *param;
        bytesoption::bytestype     type;
        bytesoption::bytesmodifier mod;
        size_t                     size;
    } commands[] = {
        { "1",  bytesoption::BYTES_MEGA,
            bytesoption::BYTES_BASE2,
            1UL * 1024 * 1024 },

        { "1k", bytesoption::BYTES_KILO,
            bytesoption::BYTES_BASE2,
            1UL * 1024 },

        { "1M", bytesoption::BYTES_SINGLE,
            bytesoption::BYTES_BASE2,
            1UL * 1024 * 1024 },

        { "1G", bytesoption::BYTES_MEGA,
            bytesoption::BYTES_BASE2,
            1UL * 1024 * 1024 * 1024 },

        { "1M", bytesoption::BYTES_SINGLE,
            bytesoption::BYTES_BASE10,
            1UL * 1000 * 1000 },

        { "1MB", bytesoption::BYTES_SINGLE,
            bytesoption::BYTES_BASE10,
            1UL * 1000 * 1000 },

        { "1MiB", bytesoption::BYTES_SINGLE,
            bytesoption::BYTES_BASE10,
            1UL * 1024 * 1024 },

    };

    static const char *argv[] = { 
        "./foo",
        "-b",
        NULL
    };

    for (unsigned int i = 0; i < elems (commands); ++i) {
        size_t size = 0;
        p->add_option(make_unique<bytesoption> (
           'b', "bytes",
           "testing sizeof bytes", &size, commands[i].type,
           commands[i].mod 
        ));

        argv[elems (argv) - 1] = commands[i].param;
        p->parse_args (elems (argv), argv);

        CHECK_EQ (commands[i].size, size);
        p->remove_option ('b');
    }
}


void
test_insert_remove_opt (void) {
    unique_ptr<processor> p(new processor ());

    {
        auto opt = make_unique<nulloption> ('n', "null-option", "null testing action");
        auto cmp = opt.get ();
        p->add_option (move (opt));
        CHECK_EQ (p->remove_option ('n').get (), (option*)cmp);
    }

    {
        auto opt = make_unique<nulloption> ('n', "null-option", "null testing action");
        auto cmp = opt.get ();
        p->add_option (move (opt));
        CHECK_EQ (p->remove_option ("null-option").get (), (option*)cmp);
    }

    {
        auto opt1 = make_unique<nulloption> ('n', "null-option", "null testing action");
        auto opt2 = make_unique<nulloption> ('n', "null-option", "null testing action");
        p->add_option (move (opt1));
        CHECK_THROWS (std::logic_error, p->add_option (move (opt2)));
    }
}


void
test_required (void) {
    unique_ptr<processor> p (new processor ());
    p->add_option (make_unique <nulloption> (
        'n',
        "null",
        "null testing",
        true
    ));

    static const char *argv[] = {
        "./cpptest",
        "-n",
        "value"
    };

    CHECK_NOTHROW (p->parse_args (elems (argv), argv));
    CHECK_THROWS  (std::runtime_error, p->parse_args (1, argv));
}


int
main (int, char **) {
    test_null_opt ();
    test_present_opt ();
    test_bool_opt ();
    test_numeric_opt< int16_t> ();
    test_numeric_opt< int32_t> ();
    test_numeric_opt< int64_t> ();
    test_numeric_opt<uint16_t> ();
    test_numeric_opt<uint32_t> ();
    test_numeric_opt<uint64_t> ();
    test_bytes_opt ();
    test_insert_remove_opt ();
    test_required ();
}
