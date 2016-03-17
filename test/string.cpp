#include <cruft/util/tap.hpp>
#include <cruft/util/string.hpp>
#include <cruft/util/types.hpp>

int
main (int, char**)
{
    util::TAP::logger tap;
    
    const char csv[] = "\0,a,123,,this is a test,";
    const std::string values[] = {
        { "\0", 1 },
        { "a" },
        { "123" },
        { "" },
        { "this is a test" },
        { "" }
    };

    std::string str (std::cbegin (csv), std::cbegin (csv) + elems (csv));
    auto tok = util::tokeniser (str, ',');
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
