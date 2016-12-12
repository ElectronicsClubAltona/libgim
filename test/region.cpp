#include "region.hpp"

#include "point.hpp"
#include "tap.hpp"


//-----------------------------------------------------------------------------
int
main (int, char **)
{
    util::TAP::logger tap;

    {
        util::point2d  ap { 32.7, -6.09703 };
        util::extent2d ae { 0.8, 2. };

        util::point2d bp {33.5, -4.5};
        util::extent2d be { 0.5, 0.5 };

        util::region2d a (ap, ae);
        util::region2d b (bp, be);

        tap.expect (!a.intersects (b), "simple 2d intersection");
    }

    tap.expect (util::region2d::max ().intersects (util::region2d::unit ()), "maximal region2d intersection");
    tap.expect (util::region2f::max ().intersects (util::region2f::unit ()), "maximal region2f intersection");

    tap.expect_eq (util::region2d::unit ().area (), 1.0,  "unit region2d area");
    tap.expect_eq (util::region2f::unit ().area (), 1.0f, "unit region2f area");

    util::point2u  p0 { 0 };
    util::extent2u e0 { 2 };

    tap.expect (util::region2u (p0, e0).includes (util::point2u {1, 1}), "unsigned region centre inclusion");
    tap.expect (util::region2u (p0, e0).includes (util::point2u {0, 0}), "unsigned region base inclusion");
    tap.expect (util::region2u (p0, e0).includes (util::point2u {2, 2}), "unsigned region corner inclusion");

    tap.expect ( util::region2u (p0, e0).contains (util::point2u {1, 1}), "unsigned region center contains");
    tap.expect (!util::region2u (p0, e0).contains (util::point2u {0, 0}), "unsigned region base contains");
    tap.expect (!util::region2u (p0, e0).contains (util::point2u {2, 2}), "unsigned region corner contains");

    //CHECK (region<2,intmax_t> (0, 0, 10, 10).includes (point2d (0.4, 0.01)));
    //CHECK (region<2,intmax_t> (0, 0, 10, 10).contains (point2d (0.4, 0.01)));

    return tap.status ();
}
