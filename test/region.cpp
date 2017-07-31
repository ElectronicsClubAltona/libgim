#include "region.hpp"

#include "point.hpp"
#include "tap.hpp"


//-----------------------------------------------------------------------------
int
main (int, char **)
{
    util::TAP::logger tap;

    // check that two overlapping regions successfully test for intersection
    {
        const util::point2d  ap { 32.7, -6.09703 };
        const util::extent2d ae { 0.8, 2. };
        const util::region2d a (ap, ae);

        const util::point2d  bp {33.5, -4.5};
        const util::extent2d be { 0.5, 0.5 };
        const util::region2d b (bp, be);

        tap.expect (!a.intersects (b), "simple 2d intersection");
    }

    // check various floating point maximums successfully test for
    // intersection. the concern is that infinities are incorrectly handled
    // in some comparisons.
    tap.expect (
        util::region2d::max ().intersects (
            util::region2d::unit ()
        ),
        "maximal region2d intersection"
    );

    tap.expect (
        util::region2f::max ().intersects (
            util::region2f::unit ()
        ),
        "maximal region2f intersection"
    );

    // ensure unit regions are... unit sized...
    tap.expect_eq (util::region2d::unit ().area (), 1.0,  "unit region2d area");
    tap.expect_eq (util::region2f::unit ().area (), 1.0f, "unit region2f area");

    // test boundary cases of includes and contains
    {
        const util::point2u  p0 { 0 };
        const util::extent2u e0 { 2 };
        const util::region2u r  {p0, e0};

        tap.expect (r.includes (util::point2u {1, 1}), "unsigned region centre inclusion");
        tap.expect (r.includes (util::point2u {0, 0}), "unsigned region base inclusion");
        tap.expect (r.includes (util::point2u {2, 2}), "unsigned region corner inclusion");

        tap.expect ( r.contains (util::point2u {1, 1}), "unsigned region center contains");
        tap.expect (!r.contains (util::point2u {0, 0}), "unsigned region base contains");
        tap.expect (!r.contains (util::point2u {2, 2}), "unsigned region corner contains");

        tap.expect ( r.has (util::point2u {1, 1}), "unsigned region centre has");
        tap.expect ( r.has (util::point2u {1, 1}), "unsigned region base has");
        tap.expect (!r.has (util::point2u {2, 2}), "unsigned region corner has");
    }

    // ensure make_union behaves as expected
    {
        const util::point2f  p { -1 };
        const util::extent2f e {  2 };
        const util::region2f r { p, e };

        tap.expect_eq (util::make_union (r, util::point2f { 0, 0 }), r, "identity union");
        tap.expect_eq (
            util::make_union (r, util::point2f { 2, 3 }),
            util::region2f { p, util::extent2f { 3, 4 } },
            "positive expanding union"
        );
        tap.expect_eq (
            util::make_union (r, util::point2f { -3, -2 }),
            util::region2f {
                util::point2f { -3, -2 },
                util::extent2f { 4, 3 }
            },
            "negative expanding union"
        );
    };

    //CHECK (region<2,intmax_t> (0, 0, 10, 10).includes (point2d (0.4, 0.01)));
    //CHECK (region<2,intmax_t> (0, 0, 10, 10).contains (point2d (0.4, 0.01)));

    return tap.status ();
}
