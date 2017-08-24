#include "geom/aabb.hpp"

#include "tap.hpp"

#include <tuple>

using util::geom::aabb2f;


int
main (int, char**)
{
    util::TAP::logger tap;

    {
        const aabb2f val { { 1, 2 }, { 8, 5 } };
        const aabb2f res { { 3, 3 }, { 6, 4 } };

        tap.expect_eq (val.contract ({2.f, 1.f}), res, "aabb::contract");
    }

    {
        const aabb2f val { { 2,  1 }, { 3, 6 } };
        const aabb2f res { { 1, -1 }, { 4, 8 } };

        tap.expect_eq (val.expand ({1.f, 2.f}), res, "aabb::expand");
    }

    return tap.status ();
}
