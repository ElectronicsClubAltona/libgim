#include "tap.hpp"

#include "vector.hpp"
#include "matrix.hpp"
#include "quaternion.hpp"

#include "coord/iostream.hpp"

#include <array>


///////////////////////////////////////////////////////////////////////////////
void
test_matrix_identities (util::TAP::logger &tap)
{
    static constexpr util::vector3f UP {0,1,0};

    {
        util::point3f p { 1, 2, 3 };

        auto m = util::translation (0-p);
        auto x = m * p.homog<4> ();
        tap.expect_eq (x, util::point4f {0,0,0,1}, "trivial translation to origin");
    }

    {
        util::point3f p {0};
        auto m = util::rotation<float> (1, { 1, 0, 0 }) *
                 util::rotation<float> (1, { 0, 1, 0 }) *
                 util::rotation<float> (1, { 0, 0, 1 });
        auto x = m * p.homog<4> ();

        tap.expect_eq (x, util::point4f {0,0,0,1}, "trivial rotation at origin");
    }

    {
        util::point3f eye { 1, 2, 3 };
        util::point3f tgt { 0, 0, 0 };

        auto m = util::look_at (eye, tgt, UP);
        auto x = m * eye.homog<4> ();
        tap.expect_eq (x, util::point4f {0,0,0,1}, "look_at eye translation");
    }

    {
        util::point3f eye { 1, 2, 3 };
        util::point3f tgt { 4, 5, 6 };

        auto m = util::look_at (eye, tgt, UP);
        auto x = m * eye.homog<4> ();
        tap.expect_eq (x, util::point4f {0,0,0,1}, "look_at eye translation with target");
    }
}


///////////////////////////////////////////////////////////////////////////////
void
test_mq_axis (util::TAP::logger &tap)
{
    static const struct {
        util::vector3f euler;
        const char *msg;
    } TESTS[] = {
        { { 1, 0, 0 }, "x-axis" },
        { { 0, 1, 0 }, "y-axis" },
        { { 0, 0, 1 }, "z-axis" },
    };

    for (auto t: TESTS) {
        auto m = util::rotation<float> (1, t.euler);
        auto q = util::quaternionf::angle_axis (1, t.euler);

        auto diff = sum (abs (m - q.as_matrix ()));
        tap.expect_le (diff, 1e-6f, "matrix/quaternion rotation identities, %s", t.msg);
    }
}


///////////////////////////////////////////////////////////////////////////////
void
test_mq_euler (util::TAP::logger &tap)
{
    static const struct {
        float x, y, z;
        const char *msg;
    } TESTS[] = {
        { 0, 0, 0, "zeroes" },
        { 1, 0, 0, "x-axis" },
        { 0, 1, 0, "y-axis" },
        { 0, 0, 1, "z-axis" },
        { 1, 1, 1, "ones"   },
        { 9, 9, 9, "nines"  }
    };

    for (auto t: TESTS) {
        const auto mx = util::rotation<float> (t.x, {1,0,0});
        const auto my = util::rotation<float> (t.y, {0,1,0});
        const auto mz = util::rotation<float> (t.z, {0,0,1});
        const auto  m = mx * my * mz;

        auto q = (
            util::quaternionf::angle_axis (t.x, { 1, 0, 0 }) *
            util::quaternionf::angle_axis (t.y, { 0, 1, 0 }) *
            util::quaternionf::angle_axis (t.z, { 0, 0, 1 })
        ).as_matrix ();

        if (abs (m - q)[0])
            exit (0);

        auto diff = util::sum (abs (m - q));
        tap.expect_le (diff, 1e-6f, "matrix-quaternion xyz euler rotations, %s", t.msg);
    }
}


///////////////////////////////////////////////////////////////////////////////
int
main (int, char**)
{
    util::TAP::logger tap;

    test_matrix_identities (tap);
    test_mq_axis  (tap);
    test_mq_euler (tap);

    return tap.status ();
}
