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

         CHECK (!a.intersects (b));
    }

    CHECK (region<double>::MAX.intersects (region<double>::UNIT));
    CHECK (region< float>::MAX.intersects (region< float>::UNIT));

    CHECK_EQ (region<double>::UNIT.area (), 1.0);
    CHECK_EQ (region< float>::UNIT.area (), 1.0f);

    CHECK (region<int> (0, 0, 2, 2).includes (point<2,int>(1, 1)));
    CHECK (region<int> (0, 0, 2, 2).includes (point<2,int>(0, 0)));
    CHECK (region<int> (0, 0, 2, 2).includes (point<2,int>(2, 2)));

    CHECK ( region<int> (0, 0, 2, 2).contains (point<2,int>(1, 1)));
    CHECK (!region<int> (0, 0, 2, 2).contains (point<2,int>(0, 0)));
    CHECK (!region<int> (0, 0, 2, 2).contains (point<2,int>(2, 2)));

    //CHECK (region<intmax_t> (0, 0, 10, 10).includes (point2d (0.4, 0.01)));
    //CHECK (region<intmax_t> (0, 0, 10, 10).contains (point2d (0.4, 0.01)));

    return 0;
}
