#include "tap.hpp"

#include "time/parse.hpp"


int
main (int, char**)
{
    util::TAP::logger tap;

    static const char* BAD[] = {
        "",
        "foo"
    };

    for (const auto &t: BAD) {
        tap.expect_throw<std::invalid_argument> (
            [t] () { util::time::iso8601::parse (t); },
            "invalid timestamp '%s'", t
        );
    }

    using namespace std::chrono_literals;

    static const struct {
        const char *string;
        std::chrono::nanoseconds value;
        const char *message;
    } DECODE[] = {
        { "1970-01-01T00:00:00Z",               0s, "UNIX epoch" },
        { "1970-01-01T01:01:00+01:01",          0s, "UNIX epoch, +offset" },
        { "1985-04-12T23:20:50.52Z",    482196050520ms, "fractional UTC time" },
        { "2000-02-28T01:00:00Z",       951699600s, "leap year, pre-29th" },
        { "2000-02-29T01:00:00Z",       951786000s, "leap year, on-29th" },
        { "2000-03-01T01:00:00Z",       951872400s, "leap year, post-29th" },
        { "1990-12-31T23:59:60Z",       662688000s, "1990 leap second in UTC" },
        { "1990-12-31T15:59:60-08:00",  662688000s, "1990 leap second in PST" },
    };

    for (const auto &t: DECODE) {
        auto val = util::time::iso8601::parse (t.string);
        tap.expect_eq (val, t.value, "%s", t.message);
    }

    tap.expect_eq (
        util::time::iso8601::parse ("1996-12-19T16:39:57-08:00"),
        util::time::iso8601::parse ("1996-12-20T00:39:57Z"),
        "timezone equivalence"
    );

    return tap.status ();
}