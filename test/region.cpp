#include "region.hpp"

#include "point.hpp"
#include "tap.hpp"

#include <vector>

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
        const util::point2f  p0 { 0 };
        const util::extent2f e0 { 2 };
        const util::region2f r  {p0, e0};

        tap.expect (!r.inclusive (util::point2f {-1, 1}), "region/point inclusive, invalid x");
        tap.expect (!r.inclusive (util::point2f { 1, 3}), "region/point inclusive, invalid y ");

        tap.expect (r.inclusive (util::point2f {1, 1}), "region/point inclusive, centre");
        tap.expect (r.inclusive (util::point2f {0, 0}), "region/point inclusive, base");
        tap.expect (r.inclusive (util::point2f {2, 2}), "region/point inclusive, corner");
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

    // ensure make_region covers the expected values
    {
        const util::region2i REGION {
            util::point2i { -1, 1 },
            util::point2i {  1, 3 }
        };

        const util::point2i EXPECTED[] = {
            { -1,  1 }, {  0,  1 }, {  1,  1 },
            { -1,  2 }, {  0,  2 }, {  1,  2 },
            { -1,  3 }, {  0,  3 }, {  1,  3 },
        };

        std::vector<util::point2i> values;
        auto sequence = util::make_range (REGION);
        std::copy (std::cbegin (sequence), std::cend (sequence), std::back_inserter (values));

        bool success = values.size () == std::size (EXPECTED)
                       && std::equal (std::cbegin (values),   std::cend (values),
                                      std::cbegin (EXPECTED), std::cend (EXPECTED));
        tap.expect (success, "make_range(region2i)");
    };

    //CHECK (region<2,intmax_t> (0, 0, 10, 10).includes (point2d (0.4, 0.01)));
    //CHECK (region<2,intmax_t> (0, 0, 10, 10).contains (point2d (0.4, 0.01)));

    return tap.status ();
}
