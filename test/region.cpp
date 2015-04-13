#include "region.hpp"

#include "debug.hpp"
#include "point.hpp"
#include "tap.hpp"

using namespace util;

int
main (int, char **) {
    {
        util::point2d  ap { 32.7, -6.09703 };
        util::extent2d ae { 0.8, 2. };

        util::point2d bp {33.5, -4.5};
        util::extent2d be { 0.5, 0.5 };

         region2d a (ap, ae);
         region2d b (bp, be);

         CHECK (!a.intersects (b));
    }

    CHECK (region2d::MAX.intersects (region2d::UNIT));
    CHECK (region2f::MAX.intersects (region2f::UNIT));

    CHECK_EQ (region2d::UNIT.area (), 1.0);
    CHECK_EQ (region2f::UNIT.area (), 1.0f);

    util::point2u p0 { 0 };
    util::extent2u e0 { 2 };

    CHECK (region2u (p0, e0).includes (point2u {1, 1}));
    CHECK (region2u (p0, e0).includes (point2u {0, 0}));
    CHECK (region2u (p0, e0).includes (point2u {2, 2}));

    CHECK ( region2u (p0, e0).contains (point2u {1, 1}));
    CHECK (!region2u (p0, e0).contains (point2u {0, 0}));
    CHECK (!region2u (p0, e0).contains (point2u {2, 2}));

    //CHECK (region<2,intmax_t> (0, 0, 10, 10).includes (point2d (0.4, 0.01)));
    //CHECK (region<2,intmax_t> (0, 0, 10, 10).contains (point2d (0.4, 0.01)));

    util::TAP::logger tap;
    tap.skip ("convert to TAP");
}
