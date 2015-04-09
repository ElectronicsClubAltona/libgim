#include "../debug.hpp"
#include "../maths.hpp"

#include <cstdlib>
#include <cmath>
#include <limits>

using std::sqrt;
using std::numeric_limits;


void
test_comparisons (void)
{
    // Check pos/neg zeroes
    CHECK (almost_equal ( 0.f,  0.f));
    CHECK (almost_equal ( 0.f, -0.f));
    CHECK (almost_equal (-0.f,  0.f));
    CHECK (almost_equal (-0.f, -0.f));

    CHECK (almost_equal ( 0.,  0.));
    CHECK (almost_equal ( 0., -0.));
    CHECK (almost_equal (-0.,  0.));
    CHECK (almost_equal (-0., -0.));

    // Check zero comparison with values near the expected cutoff
    CHECK ( almost_zero (1e-45f));
    CHECK (!almost_zero (1e-40f));
    CHECK (!exactly_zero (1e-45f));

    // Compare values a little away from zero
    CHECK (!almost_equal (-2.0, 0.0));
    CHECK (!almost_equal (-2.f, 0.f));

    // Compare values at the maximum extreme
    CHECK (!almost_equal (-std::numeric_limits<float>::max (), 0.f));
    CHECK (!almost_equal (-std::numeric_limits<float>::max (),
                           std::numeric_limits<float>::max ()));

    // Compare infinity values
    CHECK ( almost_equal (numeric_limits<double>::infinity (),
                          numeric_limits<double>::infinity ()));
    CHECK (!almost_equal (numeric_limits<double>::infinity (), 0.0));

    // Compare NaNs
    CHECK (!almost_equal (0., numeric_limits<double>::quiet_NaN ()));
    CHECK (!almost_equal (numeric_limits<double>::quiet_NaN (), 0.));

    CHECK (!almost_equal (numeric_limits<double>::quiet_NaN (),
                          numeric_limits<double>::quiet_NaN ()));
}


void
test_normalisations (void)
{
    // u8 to float
    {
        auto a = renormalise<uint8_t,float> (255);
        CHECK_EQ (a, 1.f);

        auto b = renormalise<uint8_t,float> (0);
        CHECK_EQ (b, 0.f);
    }

    // float to u8
    {
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
            CHECK_EQ ((unsigned)v, (unsigned)i.b);
        }
    }
}


int
main (int, char **) {
    // Max out the precision in case we trigger debug output
    std::cerr.precision (std::numeric_limits<double>::digits10);
    std::cout.precision (std::numeric_limits<double>::digits10);

    test_comparisons ();

    test_normalisations ();

    CHECK_EQ (util::min (-2, 0, 2), -2);
    CHECK_EQ (util::max (-2, 0, 2),  2);

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

    CHECK_EQ (to_degrees (PI_d),  180.0);
    CHECK_EQ (to_degrees (PI_f),  180.f);
    CHECK_EQ (to_radians (180.f),  PI_f);
    CHECK_EQ (to_radians (180.0),  PI_d);

    CHECK_EQ (log2 (8u), 3);
    CHECK_EQ (log2 (1u), 0);

    CHECK_EQ (log2   (9u), 3);
    CHECK_EQ (log2up (9u), 4);
    CHECK_EQ (log2up (8u), 3);
    CHECK_EQ (log2up (1u), 0);

    return EXIT_SUCCESS;
}
