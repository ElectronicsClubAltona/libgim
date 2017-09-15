#include "tap.hpp"
#include "string.hpp"
#include "types.hpp"
#include "iterator.hpp"

#include <vector>

int
main (int, char**)
{
    util::TAP::logger tap;
    
    const char csv[] = "\0,a,123,,this is a test,";

    // expected test data must be a std::string so we can check embedded
    // nulls (which are ambiguous when using a cstr).
    struct foo {
        const std::string value;
        const char *message;
    } TESTS[] = {
        { "\0",             "null" },
        { "a",              "single letter" },
        { "123",            "three digits" },
        { "",               "empty string" },
        { "this is a test", "string with spaces" },
        { "",               "trailing empty" }
    };

    for (const auto &[tok, expected]: util::zip (util::make_tokeniser (csv, ','), TESTS))
        tap.expect (equal (tok, expected.value), "%s", expected.message);

    return tap.status ();
}
