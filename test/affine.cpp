#include "tap.hpp"

#include "vector.hpp"
#include "matrix.hpp"
#include "quaternion.hpp"


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
        auto m = util::matrix4f::rotation (1, t.euler);
        auto q = util::quaternionf::rotation (1, t.euler);

        auto diff = sum (abs (m - q.as_matrix ()));
        tap.expect_le (diff, 1e-6f, "matrix/quaternion rotation identities, %s", t.msg);
    }
}


///////////////////////////////////////////////////////////////////////////////
void
test_mq_euler (util::TAP::logger &tap)
{
    static const struct {
        util::vector3f euler;
        const char *msg;
    } TESTS[] = {
        { { 0, 0, 0 }, "zeroes" },
        { { 1, 0, 0 }, "x-axis" },
        { { 0, 1, 0 }, "y-axis" },
        { { 0, 0, 1 }, "z-axis" },
        { { 1, 1, 1 }, "ones"   },
        { { 9, 9, 9 }, "nines"  }
    };

    for (auto t: TESTS) {
        auto m = util::matrix4f::rotation (t.euler[0], { 1, 0, 0 }) *
                 util::matrix4f::rotation (t.euler[1], { 0, 1, 0 }) *
                 util::matrix4f::rotation (t.euler[2], { 0, 0, 1 });
        auto q = (
            util::quaternionf::rotation (t.euler[0], { 1, 0, 0 }) *
            util::quaternionf::rotation (t.euler[1], { 0, 1, 0 }) *
            util::quaternionf::rotation (t.euler[2], { 0, 0, 1 })
        ).as_matrix ();

        auto diff = util::sum (abs (m - q));
        tap.expect_le (diff, 1e-6f, "matrix-quaternion xyz euler rotations, %s", t.msg);
    }
}


///////////////////////////////////////////////////////////////////////////////
int
main (int, char**)
{
    util::TAP::logger tap;

    test_mq_axis  (tap);
    test_mq_euler (tap);

    return tap.status ();
}
