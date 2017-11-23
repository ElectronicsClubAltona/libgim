#include "../../geom/aabb.hpp"
#include "../../geom/plane.hpp"
#include "../../geom/ray.hpp"
#include "../../tap.hpp"

using util::geom::ray2f;
using util::geom::ray3f;


//-----------------------------------------------------------------------------
void
test_intersect_plane (util::TAP::logger &tap)
{
    // trivial case: origin ray facing z, plane at unit z facing -z.
    const util::geom::ray3f l (util::point3f {0,0,0}, util::vector3f {0,0, 1});
    const util::geom::plane3f p (util::point3f {0,0,1}, util::vector3f {0,0,-1});

    tap.expect_eq (l.intersect (p), 1.f, "ray-plane intersect");
}


//-----------------------------------------------------------------------------
void
test_intersect_aabb (util::TAP::logger &tap)
{
    using util::geom::aabb2f;

    // trivial case: unit aabb at origin, ray from (0.5,-0.5) upwards
    const aabb2f box {
        { 0.f, 0.f },
        { 1.f, 1.f }
    };

    const ray2f forward {
        util::point2f { 0.5f, -0.5f },
        util::vector2f { 0.f,   1.f }
    };

    tap.expect_eq (forward.intersect (box), 0.5f, "ray-aabb intersect");

    const ray2f behind {
        util::point2f { 0.5f, 2.f },
        util::vector2f { 0.f, 1.f }
    };

    tap.expect_nan (behind.intersect (box), "ray-aabb intersect behind");
}


//-----------------------------------------------------------------------------
void
test_intersect_sphere (util::TAP::logger &tap)
{
    using util::geom::sphere3f;

    const sphere3f s = {{0.f, 0.f, 0.f}, 1.f};

    const ray3f r0 {util::point3f {0.f, 2.f, 0.f}, util::vector3f {0.f, -1.f, 0.f}};
    tap.expect_eq (r0.intersect (s), 1.f, "ray-sphere simple");

    const ray3f r1 {util::point3f {0.f, 1.f, 0.f}, util::vector3f {0.f, 1.f, 0.f}};
    tap.expect_eq (r1.intersect (s), 0.f, "ray-sphere adjacent");

    const ray3f r2 {util::point3f {0.f, 2.f, 0.f}, util::vector3f {0.f, 1.f, 0.f}};
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
