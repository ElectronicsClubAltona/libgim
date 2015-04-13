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
    const util::AABB2f b {
        { 0.f, 0.f },
        { 1.f, 1.f }
    };

    const util::ray2f l {
        { 0.5f, -0.5f },
        { 0.f,   1.f }
    };

    tap.expect_eq (l.intersect (b), 0.5f, "ray-aabb intersect");
}


//-----------------------------------------------------------------------------
void
test_intersect_sphere (util::TAP::logger &tap)
{
    ;
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
