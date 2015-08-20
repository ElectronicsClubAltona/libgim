#include "maths.hpp"

#include "debug.hpp"
#include "tap.hpp"

#include <cstdlib>
#include <cmath>
#include <limits>

using std::sqrt;
using std::numeric_limits;


void
test_comparisons (util::TAP::logger &tap)
{
    // Check pos/neg zeroes
    tap.expect (almost_equal ( 0.f,  0.f), "equal float zeros +ve/+ve");
    tap.expect (almost_equal ( 0.f, -0.f), "equal float zeros +ve/-ve");
    tap.expect (almost_equal (-0.f,  0.f), "equal float zeros -ve/+ve");
    tap.expect (almost_equal (-0.f, -0.f), "equal float zeros -ve/-ve");

    tap.expect (almost_equal ( 0.,  0.), "equal double zeroes +ve/+ve");
    tap.expect (almost_equal ( 0., -0.), "equal double zeroes +ve/+ve");
    tap.expect (almost_equal (-0.,  0.), "equal double zeroes +ve/+ve");
    tap.expect (almost_equal (-0., -0.), "equal double zeroes +ve/+ve");

    // Check zero comparison with values near the expected cutoff
    tap.expect (almost_zero (1e-45f), "almost_zero with low value");
    tap.expect (!almost_zero (1e-40f), "not almost_zero with low value");
    tap.expect (!exactly_zero (1e-45f), "not exactly_zero with low value");

    // Compare values a little away from zero
    tap.expect (!almost_equal (-2.0, 0.0), "not equal floats");
    tap.expect (!almost_equal (-2.f, 0.f), "not equal doubles");

    // Compare values at the maximum extreme
    tap.expect (!almost_equal (-std::numeric_limits<float>::max (), 0.f), "not equal -max/0 float");
    tap.expect (!almost_equal (-std::numeric_limits<float>::max (),
                                std::numeric_limits<float>::max ()),
                "not equal -max/max");

    // Compare infinity values
    tap.expect ( almost_equal (numeric_limits<double>::infinity (),
                               numeric_limits<double>::infinity ()),
                "almost_equal +infinity");
    tap.expect (!almost_equal (numeric_limits<double>::infinity (), 0.0),
                "not almost_equal +inf/0");

    // Compare NaNs
    tap.expect (!almost_equal (0., numeric_limits<double>::quiet_NaN ()), "not almost_equal double 0/NaN");
    tap.expect (!almost_equal (numeric_limits<double>::quiet_NaN (), 0.), "not almost_equal double NaN/0");

    tap.expect (!almost_equal (numeric_limits<double>::quiet_NaN (),
                               numeric_limits<double>::quiet_NaN ()),
                "not almost_equal NaN/NaN");

    // Compare reasonably close values that are wrong
    tap.expect (!almost_equal (1.0000f, 1.0001f),  ".0001f difference inequality");
    tap.expect ( almost_equal (1.0000f, 1.00001f), ".00001f difference inequality");
}


void
test_normalisations (util::TAP::logger &tap)
{
    // u8 to float
    {
        auto a = renormalise<uint8_t,float> (255);
        tap.expect_eq (a, 1.f, "normalise uint8 max");

        auto b = renormalise<uint8_t,float> (0);
        tap.expect_eq (b, 0.f, "normalise uint8 min");
    }

    // float to u8
    {
        bool success = true;

        struct {
            float   a;
            uint8_t b;
        } TESTS[] = {
            {  1.f, 255 },
            {  0.f,   0 },
            {  2.f, 255 },
            { -1.f,   0 }
        };

        for (auto i: TESTS) {
            std::cerr << "x";
            auto v = renormalise<decltype(i.a),decltype(i.b)> (i.a);
            success = success && almost_equal (unsigned{v}, unsigned{i.b});
        }

        tap.expect (success, "float-u8 normalisation");
    }
}


int
main (void)
{
    util::TAP::logger tap;

    // Max out the precision in case we trigger debug output
    std::cerr.precision (std::numeric_limits<double>::digits10);
    std::cout.precision (std::numeric_limits<double>::digits10);

    test_comparisons (tap);

    test_normalisations (tap);

    tap.expect_eq (util::min (-2, 0, 2), -2, "variadic min");
    tap.expect_eq (util::max (-2, 0, 2),  2, "variadic max");

    tap.expect_eq (pow2 (4), 16, "pow2");

    tap.expect_eq (rootsquare (2, 2), sqrt (8), "rootsquare");

    static const double POS_ZERO =  1.0 / numeric_limits<double>::infinity ();
    static const double NEG_ZERO = -1.0 / numeric_limits<double>::infinity ();

    tap.expect_eq (sign (-1), -1, "sign(-1)");
    tap.expect_eq (sign ( 1),  1, "sign( 1)");
    tap.expect_eq (sign (POS_ZERO),  1, "sign (POS_ZERO)");
    tap.expect_eq (sign (NEG_ZERO), -1, "sign (NEG_ZERO)");
    tap.expect_eq (sign ( numeric_limits<double>::infinity ()),  1, "sign +inf");
    tap.expect_eq (sign (-numeric_limits<double>::infinity ()), -1, "sign -inf");

    tap.expect_eq (to_degrees (PI< float>), 180.f, "to_degrees float");
    tap.expect_eq (to_degrees (PI<double>), 180.0, "to_degrees double");
    tap.expect_eq (to_radians (180.f), PI<float>,   "to_radians float");
    tap.expect_eq (to_radians (180.0), PI<double>,  "to_radians double");

    tap.expect_eq (log2 (8u), 3, "log2 +ve");
    tap.expect_eq (log2 (1u), 0, "log2 zero");

    //tap.expect_eq (log2   (9u), 3, "log2up 9");
    tap.expect_eq (log2up (9u), 4, "log2up 9");
    tap.expect_eq (log2up (8u), 3, "log2up 9");
    tap.expect_eq (log2up (1u), 0, "log2up 9");

    return tap.status ();
}
