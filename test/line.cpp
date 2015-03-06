#include "line.hpp"
#include "plane.hpp"
#include "debug.hpp"
#include "aabb.hpp"


void
test_intersect_plane (void)
{
    // trivial case: origin line facing z, plane at unit z facing -z.
    util::line3f  l ({0,0,0}, {0,0, 1});
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

    util::line2f l {
        { 0.5f, -0.5f },
        { 0.f,   1.f }
    };

    CHECK_EQ (l.intersect (b), 0.5f);
}


int
main (void)
{
    test_intersect_plane ();
    test_intersect_aabb ();
}
