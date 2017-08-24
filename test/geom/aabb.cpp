#include "geom/aabb.hpp"

#include "tap.hpp"

#include <tuple>

using util::geom::aabb2f;


int
main (int, char**)
{
    util::TAP::logger tap;

    {
        // Test contraction
        const aabb2f val { { 2, 2 }, { 8, 8 } };
        const aabb2f res { { 3, 3 }, { 7, 7 } };

        tap.expect_eq (val.contract (2.f), res, "over contraction");
    }

    {
        // Test expansion
        const aabb2f val { { 2, 2 }, { 8, 8 } };
        const aabb2f res { { 1, 1 }, { 9, 9 } };

        tap.expect_eq (val.expand (2.f), res, "expansion");
    }

    return tap.status ();
}
