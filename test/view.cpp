#include "tap.hpp"
#include "view.hpp"
    
int
main (int, char**)
{
    util::TAP::logger tap;

    const std::string s = "this is a test string";
    const std::string t = "not the same string";

    tap.expect (equal (s, util::make_view(s)), "string/view equality");
    tap.expect (!equal (s, util::make_view(t)), "string/view inequality");
    tap.expect (equal (s.data (), util::make_view (s)), "c-str/view equality");
    tap.expect (equal (s, util::view<const char*> (&*s.cbegin (), &*s.cend ())), "string/pointer-view equality");

    return tap.status ();
}
