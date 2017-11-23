#include "tap.hpp"
#include "alloc/raw/dynamic.hpp"
#include "alloc/raw/null.hpp"


///////////////////////////////////////////////////////////////////////////////
int
main (void)
{
    util::TAP::logger tap;

    auto obj = util::alloc::raw::dynamic::make<util::alloc::raw::null> ();

    tap.expect_throw<std::bad_alloc> (
        [&] (void) {
            obj.allocate (sizeof (char));
        },
        "trivial dispatch to null allocator"
    );

    return tap.status ();
}
