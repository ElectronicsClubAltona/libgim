#include "../debug.hpp"
#include "../maths.hpp"

#include <cstdlib>
#include <cmath>
#include <limits>

using std::sqrt;
using std::numeric_limits;

int
main (int, char **) {
    CHECK_HARD (!almost_equal (-2.0, 0.0));
    CHECK_HARD (!almost_equal (-2.f, 0.f));
    CHECK_HARD ( almost_equal ( 0.0, 0.0));
    //CHECK_HARD ( almost_equal ( 0.0, numeric_limits<double>::min ()));
    CHECK_HARD ( almost_equal (numeric_limits<double>::infinity (),
                               numeric_limits<double>::infinity ()));
    CHECK_HARD (!almost_equal (numeric_limits<double>::infinity (), 0.0));
    CHECK_HARD (!almost_equal (numeric_limits<double>::quiet_NaN (),
                               numeric_limits<double>::quiet_NaN ()));


    CHECK_EQ (min (-2, 0, 2), -2);
    CHECK_EQ (max (-2, 0, 2),  2);

    CHECK_EQ (pow2 (2),  4);
    CHECK_EQ (pow2 (4), 16);

    CHECK_EQ (rootsquare (2, 2), sqrt (8));

    double pos_zero =  1.0 / numeric_limits<double>::infinity ();
    double neg_zero = -1.0 / numeric_limits<double>::infinity ();

    CHECK_EQ (sign (-1),   -1);
    CHECK_EQ (sign ( 1),    1);
    CHECK_EQ (sign (pos_zero),  1);
    CHECK_EQ (sign (neg_zero), -1);
    CHECK_EQ (sign ( numeric_limits<double>::infinity ()),  1);
    CHECK_EQ (sign (-numeric_limits<double>::infinity ()), -1);

    CHECK_EQ (to_degrees (PI),  180);
    CHECK_EQ (to_radians (180),  PI);

    CHECK_EQ (log2 (8u), 3);
    CHECK_EQ (log2 (1u), 0);

    return EXIT_SUCCESS;
}
