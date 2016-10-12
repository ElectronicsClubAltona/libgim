#include <quaternion.hpp>

#include "tap.hpp"
#include "types.hpp"
#include "maths.hpp"
#include "coord/iostream.hpp"

using util::quaternion;
using util::quaternionf;


///////////////////////////////////////////////////////////////////////////////
int
main (void)
{
    util::TAP::logger tap;

    // identity relations
    tap.expect_eq (
        norm (quaternionf::IDENTITY), 1.f,
        "identity magnitude is unit"
    );

    tap.expect_eq (
        quaternionf::IDENTITY * quaternionf::IDENTITY,
        quaternionf::IDENTITY,
        "identity multiplication with identity"
    );

    // normalisation
    {
        auto val = normalised (quaternionf (2, 3, 4, 7));

        tap.expect_eq (
            val * quaternionf::IDENTITY,
            val,
            "identity multiplication with quaternion constant"
        );
    }

    {
        // construct two quaternions for a trivial multiplication check.
        //
        // we use vector as an intermediate form so that normalisation can be
        // done before the quaternion code is invoked (which may consider the
        // values to be invalid and throw an exception given it's geared more
        // towards rotations than general maths).
        util::vector4f a_v { 2, -11,  5, -17};
        util::vector4f b_v { 3,  13, -7, -19};

        auto a = normalised (a_v).as<quaternion> ();
        auto b = normalised (b_v).as<quaternion> ();
        auto c = quaternionf {
            -0.27358657116960006f,
            -0.43498209092420004f,
            -0.8443769181970001f,
            -0.15155515799559996f,
        };

        tap.expect_eq  (a * b, c, "multiplication");
        tap.expect_neq (b * a, c, "multiplication is not commutative");
    }

    tap.expect_eq (
        quaternionf::IDENTITY.as_matrix (),
        util::matrix4f::IDENTITY,
        "identity quaternion to matrix"
    );


    // check that concatenated transforms are identical to their matrix
    // equivalent (which we are more likely to have correct).
    {
        static const struct {
            float mag;
            util::vector3f axis;
        } ROTATIONS[] = {
            { 1.f, { 1.f, 0.f, 0.f } },
            { 1.f, { 0.f, 1.f, 0.f } },
            { 1.f, { 0.f, 0.f, 1.f } },
        };

        for (size_t i = 0; i < elems (ROTATIONS); ++i) {
            const auto &r = ROTATIONS[i];

            auto q = quaternionf::angle_axis  (r.mag, r.axis).as_matrix ();
            auto m = util::matrix4f::rotation (r.mag, r.axis);
            auto diff = util::abs (q - m);

            tap.expect_lt (util::sum (diff), 1e-6f, "single basis rotation %zu", i);
        }

        auto q = quaternionf::IDENTITY;
        auto m = util::matrix4f::IDENTITY;

        for (auto r: ROTATIONS) {
            q = q.angle_axis (r.mag, r.axis) * q;
            m = m.rotation (r.mag, r.axis) * m;
        }

        auto diff = util::abs (q.as_matrix () - m);
        tap.expect_lt (util::sum (diff), 1e-6f, "chained single axis rotations");
    }

    // ensure vector rotation quaternions actually rotate a vector
    {
        const struct {
            util::vector3f src;
            util::vector3f dst;
            const char *msg;
        } TESTS[] = {
            { {  1,  0,  0 }, {  1,  0,  0 }, "x-axis identity" },
            { {  0,  1,  0 }, {  0,  1,  0 }, "y-axis identity" },
            { {  0,  0,  1 }, {  0,  0,  1 }, "z-axis identity" },
            { {  0,  0,  1 }, {  0,  0, -1 }, "+z to -z" },
            { {  0,  0,  1 }, { -1,  0,  0 }, "+z to -x" },
            { {  1, -2,  3 }, { -4,  5, -6 }, "incremental" },
        };

        for (const auto &t: TESTS) {
            auto src = normalised (t.src);
            auto dst = normalised (t.dst);

            auto q = quaternionf::from_to (src, dst);
            auto v = rotate (src, q);

            auto diff = std::abs (util::sum (dst - v));
            tap.expect_lt (diff, 1e-06f, "quaternion from-to, %s", t.msg);
        }
    }

    return tap.status ();
}
