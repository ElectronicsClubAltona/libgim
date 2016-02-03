#include "format.hpp"

#include "tap.hpp"

int
main (void)
{
    using namespace std::string_literals;

    util::TAP::logger tap;

    tap.expect_eq (util::format::render ("identity"),        "identity"s, "identity literal");
    tap.expect_eq (util::format::render ("%s", "identity"s), "identity"s, "identity string substitution");
    tap.expect_eq (util::format::render ("%s", "identity" ), "identity"s, "identity char[] substitution");

    tap.expect_throw<util::format::missing_error> ([] (void) {
        util::format::render ("%s");
    }, "missing value");

    tap.expect_throw<util::format::format_error> ([] (void) {
        util::format::render ("%<", 42);
    }, "invalid specifier");

    tap.expect_throw<util::format::format_error> ([] (void) {
        util::format::render ("%", 42);
    }, "truncated specifier");

    return tap.status ();
}
