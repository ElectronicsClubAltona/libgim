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
void
test_spherical (util::TAP::logger &tap)
{
    static constexpr struct {
        util::vector3f spherical;
        util::vector3f cartesian;
        const char *message;
    } TESTS[] = {
        { {  1,  0,  0 }, {  0,  0,  1 }, "+zero", },
        { { -1,  0,  0 }, {  0,  0, -1 }, "-zero", },

        { {  1,  1,  0 }, {  1,  0,  0 }, "90-theta", },
        { {  1,  2,  0 }, {  0,  0, -1 }, "180-theta", },
        { {  1,  3,  0 }, { -1,  0,  0 }, "270-theta", },

        { {  1,  0,  1 }, {  0,  0,  1 }, "90-phi", },
        { {  1,  0,  2 }, {  0,  0,  1 }, "180-phi", },
        { {  1,  0,  3 }, {  0,  0,  1 }, "270-phi", },

        { {  1,  1,  1 }, {  0,  1,  0 }, "90-theta,  90-phi" },
        { {  1,  1,  2 }, { -1,  0,  0 }, "90-theta, 180-phi" },
        { {  1,  1,  3 }, {  0, -1,  0 }, "90-theta, 270-phi" },
    };

    for (const auto t: TESTS) {
        tap.expect_eq (
            util::spherical_to_cartesian (t.spherical),
            t.cartesian,
            "%s, spherical-cartesian",
            t.message
        );

        tap.expect_eq (
            util::cartesian_to_spherical (t.cartesian),
            t.spherical,
            "%s, cartesian-spherical",
            t.message
        );
    }


    {
        //util::vector3f s { 1, .5f, 2/3.f };
        //util::vector3f c { 0.35f, 0.61f, 0.71f };
        //tap.expect_eq
    }
};


///////////////////////////////////////////////////////////////////////////////
int
main ()
{
    util::TAP::logger tap;

    test_polar (tap);
    test_euler (tap);
    test_spherical (tap);

    tap.expect (!is_normalised (util::vector3f::zeros ()), "zeros isn't normalised");
    tap.expect (!is_normalised (util::vector3f::ones  ()), "ones isn't normalised");

    tap.expect_eq (
        util::hypot (util::vector3f{0,1,2} - util::vector3f{3,2,4}),
        std::sqrt (14.f),
        "vector3f hypot"
    );

    return tap.status ();
}
