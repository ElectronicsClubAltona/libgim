#include "../debug.hpp"
#include "../maths.hpp"

#include <cstdlib>
#include <cmath>


using std::sqrt;


int
main (int, char **) {
    check_hard (!almost_equal (-2.0, 0.0));
    check_hard (!almost_equal (-2.f, 0.f));

    check_eq (min (-2, 0, 2), -2);
    check_eq (max (-2, 0, 2),  2);

    check_eq (pow2 (2),  4);
    check_eq (pow2 (4), 16);

    check_eq (rootsquare (2, 2), sqrt (8));

    return EXIT_SUCCESS;
}
