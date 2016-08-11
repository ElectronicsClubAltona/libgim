#include <quaternion.hpp>

#include "tap.hpp"
#include "types.hpp"

using util::quaternion;
using util::quaternionf;


///////////////////////////////////////////////////////////////////////////////
int
main (void)
{
    util::TAP::logger tap;

    tap.expect_eq (
        norm (quaternionf::IDENTITY), 1.f,
        "identity magnitude is unit"
    );

    tap.expect_eq (
        quaternionf::IDENTITY * quaternionf::IDENTITY,
        quaternionf::IDENTITY,
        "identity multiplication with identity"
    );

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

    {
        static const struct {
            float mag;
            util::vector3f axis;
        } ROTATIONS[] = {
            { 0.f, { 1.f, 0.f, 0.f } },
            { 1.f, { 0.f, 1.f, 0.f } },
            { 0.f, { 0.f, 0.f, 1.f } },
        };

        for (size_t i = 0; i < elems (ROTATIONS); ++i) {
            const auto &r = ROTATIONS[i];

            tap.expect_eq (quaternionf::rotation    (r.mag, r.axis).as_matrix (),
                           util::matrix4f::rotation (r.mag, r.axis),
                           "single basis rotation %zu", i);
        }

        auto q_total = quaternionf::IDENTITY;
        auto m_total = util::matrix4f::IDENTITY;

        for (auto r: ROTATIONS) {
            q_total = q_total.rotation (r.mag, r.axis) * q_total;
            m_total = m_total.rotation (r.mag, r.axis) * m_total;
        }

        tap.expect_eq (q_total.as_matrix (), m_total, "chained single axis rotations");
    }

    return tap.status ();
}
