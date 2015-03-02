#include "../region.hpp"
#include "../point.hpp"
#include "../debug.hpp"

using namespace util;

int
main (int, char **) {
    {
         region2d a {32.7, -6.09703, 0.8, 2};
         region2d b {33.5, -4.5,     0.5, 0.5};

         CHECK (!a.intersects (b));
    }

    CHECK (region2d::MAX.intersects (region2d::UNIT));
    CHECK (region2f::MAX.intersects (region2f::UNIT));

    CHECK_EQ (region2d::UNIT.area (), 1.0);
    CHECK_EQ (region2f::UNIT.area (), 1.0f);

    CHECK (region2u (0, 0, 2, 2).includes (point2u {1, 1}));
    CHECK (region2u (0, 0, 2, 2).includes (point2u {0, 0}));
    CHECK (region2u (0, 0, 2, 2).includes (point2u {2, 2}));

    CHECK ( region2u (0, 0, 2, 2).contains (point2u {1, 1}));
    CHECK (!region2u (0, 0, 2, 2).contains (point2u {0, 0}));
    CHECK (!region2u (0, 0, 2, 2).contains (point2u {2, 2}));

    //CHECK (region<2,intmax_t> (0, 0, 10, 10).includes (point2d (0.4, 0.01)));
    //CHECK (region<2,intmax_t> (0, 0, 10, 10).contains (point2d (0.4, 0.01)));

    return 0;
}
