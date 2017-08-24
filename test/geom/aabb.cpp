#include "geom/aabb.hpp"

#include "tap.hpp"

#include <tuple>

using util::geom::AABB2f;


int
main (int, char**)
{
    util::TAP::logger tap;

    {
        // Test contraction
        const AABB2f val { { 2, 2 }, { 8, 8 } };
        const AABB2f res { { 3, 3 }, { 7, 7 } };

        tap.expect_eq (val.contracted (2.f), res, "over contraction");
    }

    {
        // Test expansion
        const AABB2f val { { 2, 2 }, { 8, 8 } };
        const AABB2f res { { 1, 1 }, { 9, 9 } };

        tap.expect_eq (val.expanded (2.f), res, "expansion");
    }

    return tap.status ();
}
