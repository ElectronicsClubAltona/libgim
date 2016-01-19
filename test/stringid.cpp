#include "stringid.hpp"

#include "debug.hpp"
#include "tap.hpp"

#include <cstdlib>
#include <stdexcept>

int
main (int, char**) {
    util::TAP::logger tap;

    util::stringid map;

    tap.expect_throw<std::out_of_range> ([&] { map.find ("invalid"); }, "find on empty set throws");

    auto id1 = map.add ("first");
    tap.expect_eq (id1, map.find ("first"), "single entity ID matches");

    tap.expect_throw<std::out_of_range> ([&] { map.find ("invalid"); }, "invalid find throws");

    auto id2 = map.add ("second");
    tap.expect_eq (id1 + 1, id2, "monotonically increasing IDs");
    tap.expect_eq (id1, map.find ("first"), "first element still matches");

    return tap.status ();
}
