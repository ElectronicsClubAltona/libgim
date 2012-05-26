#include "../region.hpp"
#include "../debug.hpp"

using util::region;

int
main (int, char **) {
    {
         region<double> a (32.7, -6.09703, 0.8, 2);
         region<double> b (33.5, -4.5,     0.5, 0.5);

         CHECK_HARD (!a.overlaps (b));
    }

    return 0;
}
