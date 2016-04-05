#include "tap.hpp"
#include "view.hpp"
    
int
main (int, char**)
{
    util::TAP::logger tap;

    const std::string s = "this is a test string";
    const std::string t = "not the same string";

    tap.expect_eq (s, util::view<std::string::const_iterator> (s.cbegin (), s.cend ()), "string-const_iterator inequality comparison");
    tap.expect_eq (s, util::view<const char*> (&*s.cbegin (), &*s.cend ()), "string-pointer equality comparison");

    tap.expect_neq (t, util::make_view (s), "string-view inequality");

    return tap.status ();
}
