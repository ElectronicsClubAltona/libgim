#include "tap.hpp"
#include "view.hpp"

#include <algorithm>


///////////////////////////////////////////////////////////////////////////////
int
main (int, char**)
{
    util::TAP::logger tap;

    tap.expect_eq (
        std::size (util::make_view ("foo")), 3u,
        "character array view does not include trailing null"
    );

    const std::string s = "this is a test string";
    const std::string t = "not the same string";

    tap.expect_eq  (s, util::make_view(s), "string/view equality");
    tap.expect_neq (s, util::make_view(t), "string/view inequality");
    tap.expect_eq  (s.data (), util::make_view (s), "c-str/view equality");
    tap.expect_eq  (
        s,
        util::view<const char*> (&*s.cbegin (), &*s.cend ()),
        "string/pointer-view equality");

    {
        // comparator tests
        static const struct {
            util::view<const char*> a;
            util::view<const char*> b;
            bool less;
        } TESTS[] = {
            { "b", "b", false },
            { "b", "a", false },
            { "b", "c", true  },
            { "foo", "bar", false },
            { "bar", "foo", true },
            { "banana", "banana", false },
            { "banana", "banan",  false },
            { "banan",  "banana", true  },
        };

        tap.expect (std::all_of (std::begin (TESTS), std::end (TESTS), [] (const auto &x) {
            return (x.a < x.b) == x.less;
        }), "comparator less-than");
    };

    return tap.status ();
}
