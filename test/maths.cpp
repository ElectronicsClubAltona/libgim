#include "../debug.hpp"
#include "../maths.hpp"

#include <cstdlib>
#include <cmath>
#include <limits>

using std::sqrt;
using std::numeric_limits;

int
main (int, char **) {
    check_hard (!almost_equal (-2.0, 0.0));
    check_hard (!almost_equal (-2.f, 0.f));
    check_hard ( almost_equal ( 0.0, 0.0));
    check_hard ( almost_equal ( 0.0, numeric_limits<double>::min ()));
    check_hard ( almost_equal (numeric_limits<double>::infinity (),
                               numeric_limits<double>::infinity ()));
    check_hard (!almost_equal (numeric_limits<double>::infinity (), 0.0));
    check_hard (!almost_equal (numeric_limits<double>::quiet_NaN (),
                               numeric_limits<double>::quiet_NaN ()));


    check_eq (min (-2, 0, 2), -2);
    check_eq (max (-2, 0, 2),  2);

    check_eq (pow2 (2),  4);
    check_eq (pow2 (4), 16);

    check_eq (rootsquare (2, 2), sqrt (8));

    double pos_zero =  1.0 / numeric_limits<double>::infinity ();
    double neg_zero = -1.0 / numeric_limits<double>::infinity ();

    check_eq (sign (-1),   -1);
    check_eq (sign ( 1),    1);
    check_eq (sign (pos_zero),  1);
    check_eq (sign (neg_zero), -1);
    check_eq (sign ( numeric_limits<double>::infinity ()),  1);
    check_eq (sign (-numeric_limits<double>::infinity ()), -1);

    check_eq (to_degrees (PI),  180);
    check_eq (to_radians (180),  PI);

    return EXIT_SUCCESS;
}
