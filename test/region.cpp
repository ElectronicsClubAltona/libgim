#include "../region.hpp"
#include "../point.hpp"
#include "../debug.hpp"

using util::region;
using util::point2;

int
main (int, char **) {
    {
         region<double> a (32.7, -6.09703, 0.8, 2);
         region<double> b (33.5, -4.5,     0.5, 0.5);

         CHECK_HARD (!a.overlaps (b));
    }

    CHECK_HARD (region<int> (0, 0, 2, 2).includes (point2(1.0, 1.0)));
    CHECK_HARD (region<int> (0, 0, 2, 2).includes (point2(0.0, 0.0)));
    CHECK_HARD (region<int> (0, 0, 2, 2).includes (point2(2.0, 2.0)));

    CHECK_HARD ( region<int> (0, 0, 2, 2).contains (point2(1.0, 1.0)));
    CHECK_HARD (!region<int> (0, 0, 2, 2).contains (point2(0.0, 0.0)));
    CHECK_HARD (!region<int> (0, 0, 2, 2).contains (point2(2.0, 2.0)));

    CHECK_HARD (region<intmax_t> (0, 0, 10, 10).includes (point2 (0.4, 0.01)));
    CHECK_HARD (region<intmax_t> (0, 0, 10, 10).contains (point2 (0.4, 0.01)));

    return 0;
}
