
#include "point.hpp"

#include "debug.hpp"
#include "tap.hpp"
#include "types.hpp"

using namespace util;

int
main (int, char**) {
    // Redim to lower dimension
    {
        const point3f p(0.f, 1.f, 2.f);
        const point2f q = p.redim<2> ();

        CHECK_EQ (q.data[0], p.data[0]);
        CHECK_EQ (q.data[1], p.data[1]);
    }

    // Redim to higher dimension without fill
    {
        const point2f p(0.f, 1.f);
        const point3f q = p.redim<3> ();

        CHECK_EQ (p.data[0], q.data[0]);
        CHECK_EQ (p.data[1], q.data[1]);
    }

    // Redim to higher dimension with fill
    {
        static const point4f FILL (1.f, 2.f, 3.f, 4.f);
        const point2f p (0.1f, 1.f);
        const point4f q = p.template redim<4> (FILL);

        CHECK_EQ (q.data[0], p.data[0]);
        CHECK_EQ (q.data[1], p.data[1]);
        CHECK_EQ (q.data[2], FILL.data[2]);
        CHECK_EQ (q.data[3], FILL.data[3]);
    }

    // Simple linking check for coord type casting. Relies on truncation.
    {
        const point2f pf (0.5f, 0.2f);
        const point2u pu (0,    0);

        CHECK_EQ (pf.template cast<point2u::value_type> (), pu);
    }

    // redim to 4-dimension homogenous coords
    {
        const point2f p (3, 4);
        const point4f q = p.homog<4> ();

        CHECK_EQ (q.x, 3);
        CHECK_EQ (q.y, 4);
        CHECK_EQ (q.z, 0);
        CHECK_EQ (q.w, 1);
    }

    // trivial checks for distance metrics
    {
        const point2f a (1, 2);
        const point2f b (9, 5);

        CHECK_EQ (distance2 (a, b), 73.f);
        CHECK_EQ (distance  (a, b), std::sqrt (73.f));
        CHECK_EQ (manhattan (a, b), 11.f);
        CHECK_EQ (chebyshev (a, b), 8.f);
    }

    util::TAP::logger tap;
    tap.skip ("convert to TAP");
}
