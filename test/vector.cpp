#include "vector.hpp"

#include "maths.hpp"
#include "tap.hpp"

using util::vector;
using util::vector2f;


void
test_polar (util::TAP::logger &tap)
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
            { 1.f, PI<float> / 2.f },
            { 0.f, 1.f },
            "unit length, rotated"
        },

        {
            { 1.f, 2 * PI<float> },
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

        tap.expect_lt ((in_cart - to_cart).magnitude (), 0.00001f, t.desc);

        // Compare polar representations. Make sure to normalise them first.
        auto in_polar = t.polar;
        auto to_polar = util::cartesian_to_polar (t.cartesian);

        in_polar[1] = std::fmod (in_polar[1], 2 * PI<float>);
        to_polar[1] = std::fmod (to_polar[1], 2 * PI<float>);

        tap.expect_eq (in_polar, to_polar, t.desc);
    }
}


int
main ()
{
    util::TAP::logger tap;

    test_polar (tap);

    tap.expect (!util::vector3f::ZERO.is_normalised (), "zero isn't normalised");
    tap.expect (!util::vector3f::UNIT.is_normalised (), "unit is normalised");

    return tap.status ();
}
