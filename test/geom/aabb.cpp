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
        AABB2f box {
            { 2, 2 },
            { 8, 8 }
        };

        box.contract (2.f);

        tap.expect_eq<AABB2f, AABB2f> (box, { { 3, 3 }, { 7, 7 }}, "over contraction");
    }

    {
        // Test expansion
        AABB2f box {
            { 2, 2 },
            { 8, 8 }
        };

        box.expand (2.f);

        tap.expect_eq<AABB2f, AABB2f> (box, { { 1, 1 }, { 9, 9 }}, "expansion");
    }


    {
        // Ensure we don't wrap-around on unsigned position types when contracting
        AABB2f small {
            { 0, 0 },
            { 1, 1 }
        };

        small.contract (10);

        tap.expect_eq<AABB2f, AABB2f> (small, { { 0.5f, 0.5f }, { 0.5f, 0.5f }}, "unsigned over-contract");
    }

    return tap.status ();
}
