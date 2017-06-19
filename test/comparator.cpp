#include "tap.hpp"

#include "types/comparator.hpp"

int
main (int, char**)
{
    util::TAP::logger tap;

    const auto a = { 1, 2, 3 };
    const auto b = { 1, 2, 4 };

    tap.expect (
        util::comparator::indexed<decltype(a)> ()(a, b),
        "compare initializer_list as a coordinate, success"
    );

    tap.expect (
        !util::comparator::indexed<decltype(a)> ()(b, a),
        "compare initializer_list as a coordinate, failure"
    );

    return tap.status ();
}