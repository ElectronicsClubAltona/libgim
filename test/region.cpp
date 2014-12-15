#include "../region.hpp"
#include "../point.hpp"
#include "../debug.hpp"

using util::region;
using util::point;
using util::point2d;

int
main (int, char **) {
    {
         region<double> a (32.7, -6.09703, 0.8, 2);
         region<double> b (33.5, -4.5,     0.5, 0.5);

         CHECK_HARD (!a.overlaps (b));
    }

    CHECK_HARD (region<double>::MAX.overlaps (region<double>::UNIT));
    CHECK_HARD (region< float>::MAX.overlaps (region< float>::UNIT));

    CHECK_EQ (region<double>::UNIT.area (), 1.0);
    CHECK_EQ (region< float>::UNIT.area (), 1.0f);

    CHECK_HARD (region<int> (0, 0, 2, 2).includes (point<2,int>(1, 1)));
    CHECK_HARD (region<int> (0, 0, 2, 2).includes (point<2,int>(0, 0)));
    CHECK_HARD (region<int> (0, 0, 2, 2).includes (point<2,int>(2, 2)));

    CHECK_HARD ( region<int> (0, 0, 2, 2).contains (point<2,int>(1, 1)));
    CHECK_HARD (!region<int> (0, 0, 2, 2).contains (point<2,int>(0, 0)));
    CHECK_HARD (!region<int> (0, 0, 2, 2).contains (point<2,int>(2, 2)));

    //CHECK_HARD (region<intmax_t> (0, 0, 10, 10).includes (point2d (0.4, 0.01)));
    //CHECK_HARD (region<intmax_t> (0, 0, 10, 10).contains (point2d (0.4, 0.01)));

    return 0;
}
