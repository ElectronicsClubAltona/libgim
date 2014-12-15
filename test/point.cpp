
#include "point.hpp"

#include "debug.hpp"
#include "types.hpp"

using namespace util;

int
main (int, char**) {
    const point3f p(0.f, 1.f, 2.f);
    const point2f q = p.redim<2> ();

    CHECK_EQ (q.data[0], p.data[0]);
    CHECK_EQ (q.data[1], p.data[1]);
}
