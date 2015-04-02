#include "vector.hpp"

#include "maths.hpp"

using util::vector;
using util::vector2f;


void
test_polar (void)
{
    static const struct {
        util::vector2f polar;
        util::vector2f cartesian;
        const char *desc;
    } TESTS[] {
        {
            { 0.f, 0.f },
            { 0.f, 0.f },
            "all zeroes"
        },

        {
            { 1.f, 0.f },
            { 1.f, 0.f },
            "unit length, unrotated"
        },

        {
            { 1.f, PI_f / 2.f },
            { 0.f, 1.f },
            "unit length, rotated"
        },

        {
            { 1.f, 2 * PI_f },
            { 1.f, 0.f },
            "full rotation, unit length"
        }
    };

    for (const auto &t: TESTS) {
        // Compare the difference of cartesian representations. Don't use
        // direct equality comparisons here as the numeric stability can be
        // poor and we have nice whole numbers to start with.
        auto in_cart = t.cartesian;
        auto to_cart = util::polar_to_cartesian (t.polar);

        CHECK_LT ((in_cart - to_cart).magnitude (), 0.00001f);

        // Compare polar representations. Make sure to normalise them first.
        auto in_polar = t.polar;
        auto to_polar = util::cartesian_to_polar (t.cartesian);

        in_polar[1] = std::fmod (in_polar[1], 2 * PI_f);
        to_polar[1] = std::fmod (to_polar[1], 2 * PI_f);

        CHECK_EQ (in_polar, to_polar);
    }
}


int
main ()
{
    test_polar ();
}
