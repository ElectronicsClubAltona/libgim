#include "tap.hpp"
#include "string.hpp"
#include "types.hpp"

int
main (int, char**)
{
    util::TAP::logger tap;

    // the string_literal prefix is required to (easily) construct a string
    // with an internal null character.
    using namespace std::literals::string_literals;
    const std::string csv = "\0,a,123,,this is a test,"s;

    const std::string values[] = {
        { "\0", 1 },
        { "a" },
        { "123" },
        { "" },
        { "this is a test" },
        { "" }
    };

    auto tok = util::make_tokeniser (csv, ',');
    auto t_cursor = tok.cbegin ();
    auto v_cursor = std::cbegin (values);

    tap.expect_eq (*t_cursor++, *v_cursor++, "tokeniser, single letter");
    tap.expect_eq (*t_cursor++, *v_cursor++, "tokeniser, three digits");
    tap.expect_eq (*t_cursor++, *v_cursor++, "tokeniser, embedded null");
    tap.expect_eq (*t_cursor++, *v_cursor++, "tokeniser, empty string");
    tap.expect_eq (*t_cursor++, *v_cursor++, "tokeniser, string with spaces");
    tap.expect_eq (*t_cursor++, *v_cursor++, "tokeniser, trailing empty");

    tap.expect_eq (t_cursor, tok.cend (), "tokeniser iterator at end");

    return tap.status ();
}
