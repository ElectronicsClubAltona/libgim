#include "json2/event.hpp"
#include "json2/except.hpp"
#include "tap.hpp"

#include <vector>
#include <cstring>
#include <functional>


void
test_numbers (util::TAP::logger &tap)
{
    static const struct {
        const char *data;
        bool good;
        const char *message;
    } TESTS[] = {
        { "1", true, "single digit" },
        { "01", false, "leading zero" },
        { "-1", true, "leading minus" },
        { "1.", false, "truncated fraction" },
        { "1.0", true, "fraction" },
        { "1.0e", false, "truncated exponential" },
        { "1.0e1", true, "lower exponential" },
        { "1.0E1", true, "upper exponential" },
        { "1.0e+1", true, "positive exponential" },
        { "1.0e+", false, "truncated positive exponential" },
        { "1.0e-1", true, "negative exponential" },
        { "1.0e-", false, "truncated negative exponential" },
    };

    for (const auto &t: TESTS) {
        auto first = t.data;
        auto last  = first + strlen (first);

        if (!t.good) {
            tap.expect_throw<util::json2::error> (
                [&] () {
                    util::json2::event::parse ([] (auto) {}, first, last);
                },
                "number, %s",
                t.message
            );
        } else {
            util::json2::event::type_t type;
            auto end = util::json2::event::parse (
                [&type] (auto p) { type = p.type (); }, first, last
            );

            tap.expect (
                last == end && type == util::json2::event::type_t::NUMBER,
                "number, %s",
                t.message
            );
        }
    }
}


void
test_literals (util::TAP::logger &tap)
{
    static const struct {
        const char *data;
        bool good;
        util::json2::event::type_t type;
        const char *message;
    } TESTS[] = {
        { "true", true,  util::json2::event::type_t::BOOLEAN, "lower true" },
        { "TRUE", false, util::json2::event::type_t::BOOLEAN, "upper true" },
        { "truE", false, util::json2::event::type_t::BOOLEAN, "mixed true" },
        { "tru",  false, util::json2::event::type_t::BOOLEAN, "truncated true" },

        { "false", true, util::json2::event::type_t::BOOLEAN, "lower false" },
        { "FALSE", false, util::json2::event::type_t::BOOLEAN, "upper false" },
        { "falSe", false, util::json2::event::type_t::BOOLEAN, "mixed false" },
        { "fals",  false, util::json2::event::type_t::BOOLEAN, "truncated false" },

        { "null", true, util::json2::event::type_t::NONE, "lower null" },
        { "NULL", false, util::json2::event::type_t::NONE, "upper null" },
        { "nUll", false, util::json2::event::type_t::NONE, "mixed null" },
        { "nul",  false, util::json2::event::type_t::NONE, "truncated null" },
    };

    for (const auto &t: TESTS) {
        auto first = t.data;
        auto last  = first + strlen (first);

        if (t.good) {
            util::json2::event::type_t type;
            util::json2::event::parse ([&type] (auto p) { type = p.type (); }, first, last);
            tap.expect_eq (type, t.type, "literal, %s", t.message);
        } else {
            tap.expect_throw<util::json2::parse_error> (
                [first,last] () {
                    util::json2::event::parse ([] (auto) {}, first, last);
                },
                "literal, %s",
                t.message
            );
        }
    }
}


void
test_strings (util::TAP::logger &tap)
{
    static const struct {
        const char *data;
        bool good;
        const char *message;
    } TESTS[] = {
        { "\"abc\"", true, "abc" },
        { "\"\"", true, "empty" },
        { "\"", false, "unbalanced quote" },
        { "\"\\n\"", true, "newline escape" },
        { "\"\\a\"", true, "valid unnecessary escape" },
        { "\"\\uABCD\"", true, "upper unicode hex escape" },
        { "\"\\uabcd\"", true, "lower unicode hex escape" },
        { "\"\\uab\"", false, "truncated unicode hex escape" },
        { "\"\\uabxy\"", false, "invalid unicode hex escape" },
    };

    for (const auto &t: TESTS) {
        auto first = t.data;
        auto last  = first + strlen (first);

        if (t.good) {
            util::json2::event::type_t type;
            util::json2::event::parse ([&type] (auto p) { type = p.type (); }, first, last);
            tap.expect_eq (type, util::json2::event::type_t::STRING, "string, %s", t.message);
        } else {
            tap.expect_throw<util::json2::parse_error> (
                [first,last] () {
                    util::json2::event::parse ([] (auto) {}, first, last);
                },
                "string, %s",
                t.message
            );
        }
    }
};


///////////////////////////////////////////////////////////////////////////////
void
test_arrays (util::TAP::logger &tap)
{
    using util::json2::event::type_t;

    static const struct {
        const char *data;
        bool good;
        std::vector<util::json2::event::type_t> types;
        const char *message;
    } TESTS[] = {
        { "[]", true, { type_t::ARRAY_BEGIN, type_t::ARRAY_END }, "empty" },
        { "[1]", true, { type_t::ARRAY_BEGIN, type_t::NUMBER, type_t::ARRAY_END }, "single number" },
        { "[1true]", false, { type_t::ARRAY_BEGIN, type_t::NUMBER }, "contatenated number/bool" },
        { "[1,2]", true, { type_t::ARRAY_BEGIN, type_t::NUMBER, type_t::NUMBER, type_t::ARRAY_END }, "two numbers" },
        { "[1,]", false, { type_t::ARRAY_BEGIN, type_t::NUMBER }, "single trailing comma" },
        { "[1,2,]", false, { type_t::ARRAY_BEGIN, type_t::NUMBER, type_t::NUMBER }, "double trailing comma" },
        { "[,]", false, { type_t::ARRAY_BEGIN }, "only comma" },
        { "[", false, { type_t::ARRAY_BEGIN }, "missing terminator" },
        { "[[]]", true, { type_t::ARRAY_BEGIN, type_t::ARRAY_BEGIN, type_t::ARRAY_END, type_t::ARRAY_END }, "nested array" },
        { "[[]", false, { type_t::ARRAY_BEGIN, type_t::ARRAY_END }, "unbalanced nested array" },
    };

    for (const auto &t: TESTS) {
        auto first = t.data;
        auto last  = first + strlen (first);

        if (t.good) {
            std::vector<type_t> types;
            util::json2::event::parse ([&types] (auto p) { types.push_back (p.type ()); }, first, last);
            tap.expect_eq (types, t.types, "array, %s", t.message);
        } else {
            tap.expect_throw<util::json2::parse_error> (
                [&] () {
                    util::json2::event::parse ([] (auto) { }, first, last);
                }, "array, %s", t.message
            );
        }
    }
};


///////////////////////////////////////////////////////////////////////////////
void
test_objects (util::TAP::logger &tap)
{
    using util::json2::event::type_t;

    static const struct {
        const char *data;
        bool good;
        std::vector<type_t> types;
        std::vector<std::string> strings;
        const char *message;
    } TESTS[] = {
        { "{}", true, { type_t::OBJECT_BEGIN, type_t::OBJECT_END }, {}, "empty" },
        { "{", false, { type_t::OBJECT_BEGIN }, {}, "missing terminator" },
        { "{\"a\":1}", true, { type_t::OBJECT_BEGIN, type_t::STRING, type_t::NUMBER, type_t::OBJECT_END }, {"\"a\""}, "empty" },
        { "{1:1}", false, { type_t::OBJECT_BEGIN }, {}, "integer key" },
        { "{:1}", false, { type_t::OBJECT_BEGIN }, {}, "no key" },
        { "{\"a\":}", false, { type_t::OBJECT_BEGIN, type_t::STRING }, {}, "no value" },
        {
            R"json({"a":[]})json",
            true,
            {
                type_t::OBJECT_BEGIN,
                type_t::STRING,
                type_t::ARRAY_BEGIN,
                type_t::ARRAY_END,
                type_t::OBJECT_END
            },
            { "\"a\"" },
            "array value" },
        {
            R"json({ "a": { "b": null } })json",
            true,
            {
                type_t::OBJECT_BEGIN,
                type_t::STRING,
                type_t::OBJECT_BEGIN,
                type_t::STRING,
                type_t::NONE,
                type_t::OBJECT_END,
                type_t::OBJECT_END
            },
            { "\"a\"", "\"b\"" },
            "recursive object"
        }
    };

    for (const auto &t: TESTS) {
        auto first = t.data;
        auto last  = first + strlen (first);

        if (t.good) {
            std::vector<type_t> types;
            std::vector<std::string> strings;

            util::json2::event::parse ([&] (auto p) {
                types.push_back (p.type ());
                if (p.type () == type_t::STRING)
                    strings.push_back (std::string {p.first, p.last});
            }, first, last);

            tap.expect (types == t.types && strings == t.strings, "object, %s", t.message);
        } else {
            tap.expect_throw<util::json2::parse_error> (
                [&] () {
                    util::json2::event::parse ([] (auto) {}, first, last);
                }, "object, %s", t.message
            );
        }
    }
};


///////////////////////////////////////////////////////////////////////////////
int
main (void)
{
    util::TAP::logger tap;

    test_numbers (tap);
    test_literals (tap);
    test_strings (tap);
    test_arrays (tap);
    test_objects (tap);

    return tap.status ();
}