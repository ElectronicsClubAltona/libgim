#include "tap.hpp"
#include "view.hpp"
    
int
main (int, char**)
{
    util::TAP::logger tap;

    const std::string s = "this is a test string";
    const std::string t = "not the same string";

    tap.expect_eq (s, util::make_view(s), "string/view equality");
    tap.expect_neq (s, util::make_view(t), "string/view inequality");
    tap.expect_eq (s.data (), util::make_view (s), "c-str/view equality");
    tap.expect_eq (s, util::view<const char*> (&*s.cbegin (), &*s.cend ()), "string/pointer-view equality");

    return tap.status ();
}
