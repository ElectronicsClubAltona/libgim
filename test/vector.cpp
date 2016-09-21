#include "vector.hpp"

#include "maths.hpp"
#include "tap.hpp"

using util::vector;
using util::vector2f;


///////////////////////////////////////////////////////////////////////////////
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
            { 1.f, util::PI<float> / 2.f },
            { 0.f, 1.f },
            "unit length, rotated"
        },

        {
            { 1.f, 2 * util::PI<float> },
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

        tap.expect_lt (norm (in_cart - to_cart), 0.00001f, "%s", t.desc);

        // Compare polar representations. Make sure to normalise them first.
        auto in_polar = t.polar;
        auto to_polar = util::cartesian_to_polar (t.cartesian);

        in_polar[1] = std::fmod (in_polar[1], 2 * util::PI<float>);
        to_polar[1] = std::fmod (to_polar[1], 2 * util::PI<float>);

        tap.expect_eq (in_polar, to_polar, "%s", t.desc);
    }
}


///////////////////////////////////////////////////////////////////////////////
void
test_euler (util::TAP::logger &tap)
{
    static const struct {
        util::vector3f dir;
        util::vector2f euler;
        const char *name;
    } TESTS[] = {
        // y-axis
        { {  0,  0, -1 }, { 0.5f,  0.5f }, "forward" },
        { { -1,  0,  0 }, { 0.5f, -1.0f }, "left" },
        { {  0,  0,  1 }, { 0.5f, -0.5f }, "back" },
        { {  1,  0,  0 }, { 0.5f,  0.0f }, "right" },

        // x-axis
        { {  0,  1,  0 }, { 0, 0 }, "up"   },
        { {  0, -1,  0 }, { 1, 0 }, "down" },
    };

    // check that simple axis rotations look correct
    for (auto i: TESTS) {
        tap.expect_eq (util::to_euler (i.dir),
                       i.euler * util::PI<float>,
                       "to euler, %s", i.name);
    }

    // check error in round trip through euler angles
    for (auto i: TESTS) {
        auto trip = util::from_euler (util::to_euler (i.dir));
        auto diff = i.dir - trip;
        auto mag  = norm (diff);

        // trig functions reduce precision above almost_equal levels, so we
        // hard code a fairly low bound here instead.
        tap.expect_lt (mag, 1e-7, "euler round-trip error, %s", i.name);
    }
}


///////////////////////////////////////////////////////////////////////////////
int
main ()
{
    util::TAP::logger tap;

    test_polar (tap);
    test_euler (tap);

    tap.expect (!is_normalised (util::vector3f::ZERO), "zero isn't normalised");
    tap.expect (!is_normalised (util::vector3f::ONES), "ones isn't normalised");

    tap.expect_eq (
        util::hypot (util::vector3f{0,1,2} - util::vector3f{3,2,4}),
        std::sqrt (14.f),
        "vector3f hypot"
    );

    return tap.status ();
}
