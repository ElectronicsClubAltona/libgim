
#include "point.hpp"

#include "debug.hpp"
#include "types.hpp"

using namespace util;

int
main (int, char**) {
    const point<3> p(0.0, 1.0, 2.0);
    const point<2> q = p.redim<2> ();

    CHECK_EQ (q.data[0], p.data[0]);
    CHECK_EQ (q.data[1], p.data[1]);
}
