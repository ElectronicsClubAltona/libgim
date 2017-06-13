#include "point.hpp"

#include "debug.hpp"
#include "tap.hpp"
#include "types.hpp"

using namespace util;

int
main (void)
{
    util::TAP::logger tap;

    // Redim to lower dimension
    {
        const point3f p { 0.f, 1.f, 2.f };
        const point2f q = p.redim<2> ();
        const point2f r { 0.f, 1.f };

        tap.expect_eq (q, r, "redim to lower dimension");
    }

    // Redim to higher dimension without fill
    {
        const point2f p(0.f, 1.f);
        const point2f q = p.redim<2> ();

        // GCC: we can't use the equality operator here because it triggers
        // an ICE on GCC 7.1.0
        tap.expect (
            std::equal (std::cbegin (p), std::cend (p), std::cbegin (q)),
            "redim to higher dimension"
        );
    }

    // Redim to higher dimension with fill
    {
        static const point4f FILL (1.f, 2.f, 3.f, 4.f);
        const point2f p (0.1f, 1.f);
        const point4f q = p.template redim<4> (FILL);

        tap.expect (
            almost_equal (q[0], p[0]) &&
            almost_equal (q[1], p[1]) &&
            almost_equal (q[2], FILL[2]) &&
            almost_equal (q[3], FILL[3]),

            "redim to higher with fill"
        );
    }

    // Simple linking check for coord type casting. Relies on truncation.
    {
        const point2f pf (0.5f, 0.2f);
        const point2u pu (0,    0);

        tap.expect_eq (pf.template cast<point2u::value_type> (), pu, "type cast with truncation");
    }

    // redim to 4-dimension homogenous coords
    {
        const point2f p (3, 4);
        const point4f q = p.homog<4> ();

        tap.expect (
            almost_equal (q.x, 3.f) &&
            almost_equal (q.y, 4.f) &&
            almost_equal (q.z, 0.f) &&
            almost_equal (q.w, 1.f),

            "homogenous redim"
        );
    }

    // trivial checks for distance metrics
    {
        const point2f a (1, 2);
        const point2f b (9, 5);

        tap.expect_eq (distance2 (a, b), 73.f, "distance2");
        tap.expect_eq (distance  (a, b), std::sqrt (73.f), "distance");
        tap.expect_eq (manhattan (a, b), 11.f, "manhattan");
        tap.expect_eq (chebyshev (a, b), 8.f, "chebyshev");
    }

    return tap.status ();
}
