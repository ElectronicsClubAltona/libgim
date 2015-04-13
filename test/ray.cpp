#include "aabb.hpp"
#include "debug.hpp"
#include "plane.hpp"
#include "ray.hpp"
#include "tap.hpp"


void
test_intersect_plane (void)
{
    // trivial case: origin ray facing z, plane at unit z facing -z.
    util::ray3f  l ({0,0,0}, {0,0, 1});
    util::plane3f p ({0,0,1}, {0,0,-1});

    CHECK_EQ (l.intersect (p), 1);
}


void
test_intersect_aabb (void)
{
    // trivial case: unit aabb at origin, ray from (0.5,-0.5) upwards
    util::AABB2f b {
        { 0.f, 0.f },
        { 1.f, 1.f }
    };

    util::ray2f l {
        { 0.5f, -0.5f },
        { 0.f,   1.f }
    };

    CHECK_EQ (l.intersect (b), 0.5f);
}


void
test_intersect_sphere (void)
{
    ;
}


int
main (void)
{
    test_intersect_plane ();
    test_intersect_aabb ();

    util::TAP::logger tap;
    tap.todo ("convert to TAP");
}
