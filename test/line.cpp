#include "line.hpp"
#include "plane.hpp"
#include "debug.hpp"

int
main (void)
{
    // trivial case: origin line facing z, plane at unit z facing -z.
    util::line3f  l ({0,0,0}, {0,0, 1});
    util::plane3f p ({0,0,1}, {0,0,-1});

    CHECK_EQ (l.intersect (p), 1);
}
