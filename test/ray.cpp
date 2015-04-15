#include "aabb.hpp"
#include "debug.hpp"
#include "plane.hpp"
#include "ray.hpp"
#include "tap.hpp"


//-----------------------------------------------------------------------------
void
test_intersect_plane (util::TAP::logger &tap)
{
    // trivial case: origin ray facing z, plane at unit z facing -z.
    const util::ray3f  l ({0,0,0}, {0,0, 1});
    const util::plane3f p ({0,0,1}, {0,0,-1});

    tap.expect_eq (l.intersect (p), 1.f, "ray-plane intersect");
}


//-----------------------------------------------------------------------------
void
test_intersect_aabb (util::TAP::logger &tap)
{
    // trivial case: unit aabb at origin, ray from (0.5,-0.5) upwards
    const util::AABB2f box {
        { 0.f, 0.f },
        { 1.f, 1.f }
    };

    const util::ray2f forward {
        { 0.5f, -0.5f },
        { 0.f,   1.f }
    };

    tap.expect_eq (forward.intersect (box), 0.5f, "ray-aabb intersect");

    const util::ray2f behind {
        { 0.5f, 2.f },
        { 0.f, 1.f }
    };

    tap.expect_nan (behind.intersect (box), "ray-aabb intersect behind");
}


//-----------------------------------------------------------------------------
void
test_intersect_sphere (util::TAP::logger &tap)
{
    const util::sphere3f s = {{0.f, 0.f, 0.f}, 1.f};

    const util::ray3f r0 {{0.f, 2.f, 0.f}, {0.f, -1.f, 0.f}};
    tap.expect_eq (r0.intersect (s), 1.f, "ray-sphere simple");

    const util::ray3f r1 {{0.f, 1.f, 0.f}, {0.f, 1.f, 0.f}};
    tap.expect_eq (r1.intersect (s), 0.f, "ray-sphere adjacent");

    const util::ray3f r2 {{0.f, 2.f, 0.f}, {0.f, 1.f, 0.f}};
    tap.expect_nan (r2.intersect (s), "ray-sphere no-intersect");
}


//-----------------------------------------------------------------------------
int
main (void)
{
    util::TAP::logger tap;

    test_intersect_plane (tap);
    test_intersect_aabb (tap);
    test_intersect_sphere (tap);

    return tap.status ();
}
