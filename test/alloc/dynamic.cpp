
#include "tap.hpp"
#include "alloc/dynamic.hpp"
#include "alloc/null.hpp"

int
main (void)
{
    util::TAP::logger tap;

    auto obj = util::alloc::dynamic::make<util::alloc::null> ();

    tap.expect_throw<std::bad_alloc> (
        [&] (void) {
            obj.allocate (sizeof (char));
        },
        "trivial dispatch to null allocator"
    );

    return tap.status ();
}
