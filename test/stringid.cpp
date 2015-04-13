#include "stringid.hpp"

#include "debug.hpp"
#include "tap.hpp"

#include <cstdlib>
#include <stdexcept>

int
main (int, char**) {
    util::stringid map;

    CHECK_THROWS (std::out_of_range, map.find ("invalid"));

    auto id1 = map.add ("first");
    CHECK_EQ (id1, map.find ("first"));

    CHECK_THROWS (std::out_of_range, map.find ("invalid"));

    auto id2 = map.add ("second");
    CHECK_EQ (id1 + 1, id2);
    CHECK_EQ (id1, map.find ("first"));

    util::TAP::logger tap;
    tap.skip ("convert to TAP");
}
