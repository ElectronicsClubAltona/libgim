#include "bezier.hpp"

#include "tap.hpp"

#include <cstdlib>


//-----------------------------------------------------------------------------
template <size_t> void test_eval (util::TAP::logger&);
template <size_t> void test_intersect (util::TAP::logger&);
template <size_t> void test_region (util::TAP::logger&);


//-----------------------------------------------------------------------------
template <>
void
test_eval<1> (util::TAP::logger &tap)
{
    static const util::bezier<1> b1 ({{  0.f,   0.f},
                                      {100.f, 100.f}});

    auto p0 = b1.eval(0.f);
    auto p1 = b1.eval(1.f);

    tap.expect_eq (p0, b1[0], "eval bezier-1 extrema");
    tap.expect_eq (p1, b1[1], "eval bezier-1 extrema");

    auto px = b1.eval(0.5f);
    auto rx = b1[0] + 0.5f * (b1[1]-b1[0]);

    tap.expect_eq (px, rx, "eval bezier-1 midpoint");
}


//-----------------------------------------------------------------------------
template <>
void
test_eval<2> (util::TAP::logger &tap)
{
    static const util::bezier<2> b2 ({{  0.f,   0.f},
                                      { 50.f,  50.f},
                                      {100.f, 100.f}});

    auto p0 = b2.eval(0.f);
    auto p2 = b2.eval(1.f);

    tap.expect_eq (p0, b2[0], "eval bezier-2 extrema");
    tap.expect_eq (p2, b2[2], "eval bezier-2 extrema");

    auto px = b2.eval(0.5f);
    auto rx = b2[0] + 0.5f * (b2[2]-b2[0]);

    tap.expect_eq (px, rx, "eval bezier-2 midpoint");
}


//-----------------------------------------------------------------------------
template <>
void
test_eval<3> (util::TAP::logger &tap)
{
    static const util::bezier<3> b3 ({{   0.f,   0.f },
                                      {  33.f,  33.f },
                                      {  67.f,  67.f },
                                      { 100.f, 100.f }});

    auto p0 = b3.eval (0.f);
    auto p3 = b3.eval (1.f);

    tap.expect_eq (p0, b3[0], "eval bezier-3 extrema");
    tap.expect_eq (p3, b3[3], "eval bezier-3 extrema");

    auto px = b3.eval (0.5f);
    auto rx = b3[0] + 0.5f * (b3[3] - b3[0]);

    tap.expect_eq (px, rx, "eval bezier-3 midpoint");
}


//-----------------------------------------------------------------------------
template <>
void
test_intersect<1> (util::TAP::logger &tap)
{
    // A line from (0,0) to (100,100)
    static const util::bezier<1> b1 ({{0.f, 0.f}, {100.f, 100.f}});

    // Through the centre
    tap.expect_eq (b1.intersections ({0.f, 100.f}, {100.f, 0.f}),
                   1u,
                   "intersect bezier-1 centre");
    tap.expect_eq (b1.intersections ({100.f, 0.f}, {0.f, 100.f}),
                   1u,
                   "intersect bezier-1 centre");

    // Coincident with endpoints
    tap.expect_eq (b1.intersections ({0.f, 0.f}, {1.f,0.f}),
                   1u,
                   "intersect bezier-1 endpoints");
    tap.expect_eq (b1.intersections ({100.f, 100.f}, {100.f,0.f}),
                   1u,
                   "intersect bezier-1 endpoints");

    // Co-planar
    tap.expect_eq (b1.intersections ({0.f, 0.f}, {1.f, 1.f}),
                   1u,
                   "intersect bezier-1 co-planar");

    // Underneath
    tap.expect_eq (b1.intersections ({1000.f, -10.f}, {-1000.f, -10.f}),
                   0u,
                   "intersect bezier-1 under");

    // Above
    tap.expect_eq (b1.intersections ({1000.f, 110.f}, {-1000.f, 110.f}),
                   0u,
                   "intersect bezier-1 above");

}

//-----------------------------------------------------------------------------
template <>
void
test_intersect<2> (util::TAP::logger &tap)
{
    // A linear curve from (0,0) to (100,100)
    static const util::bezier<2> b2 ({{ 0.0f,  0.0f},
                                      { 50.f,  50.f},
                                      {100.f, 100.f}});

    // Through the centre
    tap.expect_eq (b2.intersections ({100.f, 0.f}, {0.f, 100.f}), 1, "intersect bezier-2 centre");
    tap.expect_eq (b2.intersections ({0.f, 100.f}, {100.f, 0.f}), 1, "intersect bezier-2 centre");

    // Coincident with endpoints
    tap.expect_eq (b2.intersections ({0.f, 0.f}, {0.f,100.f}), 1, "intersect bezier-2 endpoint");
    tap.expect_eq (b2.intersections ({0.f, 0.f}, {100.f,0.f}), 1, "intersect bezier-2 endpoint");
    tap.expect_eq (b2.intersections ({100.f, 100.f}, {100.f,0.f}), 1, "intersect bezier-2 endpoint");

    // Co-planar
    tap.expect_eq (b2.intersections ({0.f, 0.f}, {1.f, 1.f}), 1, "intersect bezier-2 co-planar");

    // Underneath
    tap.expect_eq (b2.intersections ({1000.f, -10.f}, {-1000.f, -10.f}), 0, "intersect bezier-2 under");

    // Above
    tap.expect_eq (b2.intersections ({1000.f, 110.f}, {-1000.f, 110.f}), 0, "intersect bezier-2 above");
}


//-----------------------------------------------------------------------------
template <>
void
test_intersect<3> (util::TAP::logger &tap)
{
    // A linear curve from (0,0) to (100,100)
    static const util::bezier<3> b3 ({{   0.f,   0.f },
                                      {  33.f,  33.f },
                                      {  67.f,  67.f },
                                      { 100.f, 100.f }});

    // Through the centre
    tap.expect_eq (b3.intersections ({100.f, 0.f}, {0.f, 100.f}), 1, "intersect bezier-3 centre");

    // Coincident with endpoints
    tap.expect_eq (b3.intersections ({0.f, 0.f}, {0.f,100.f}), 1, "intersect bezier-3 endpoint");
    tap.expect_eq (b3.intersections ({0.f, 0.f}, {100.f,0.f}), 1, "intersect bezier-3 endpoint");
    tap.expect_eq (b3.intersections ({100.f, 100.f}, {100.f,0.f}), 1, "intersect bezier-3 endpoint");

    // Co-planar
    tap.expect_eq (b3.intersections ({0.f, 0.f}, {1.f, 1.f}), 1, "intersect bezier-3 co-planar");

    // Underneath
    tap.expect_eq (b3.intersections ({1000.f, -10.f}, {-1000.f, -10.f}), 0, "intersect bezier-3 under");

    // Above
    tap.expect_eq (b3.intersections ({1000.f, 110.f}, {-1000.f, 110.f}), 0, "intersect bezier-3 above");
}


//-----------------------------------------------------------------------------
template <>
void
test_region<1> (util::TAP::logger &tap)
{
    util::point2f p0 {   0,   0 },
                  p1 { 100,   0 },
                  p2 { 100, 100 },
                  p3 {   0, 100 };

    static const util::bezier<1> upright    ({p0, p2});
    static const util::bezier<1> downleft   ({p2, p0});
    static const util::bezier<1> vertical   ({p0, p3});
    static const util::bezier<1> horizontal ({p0, p2});

    tap.expect_eq (upright.region (),    util::region2f (p0, p2), "bezier-1 region");
    tap.expect_eq (downleft.region (),   util::region2f (p0, p2), "bezier-1 region");
    tap.expect_eq (vertical.region (),   util::region2f (p0, p3), "bezier-1 region");
    tap.expect_eq (horizontal.region (), util::region2f (p0, p2), "bezier-1 region");
}


//-----------------------------------------------------------------------------
template <>
void
test_region<2> (util::TAP::logger &tap)
{
    util::point2f p0 {   0,   0 },
                  p1 {  50,  50 },
                  p2 { 100, 100 };

    static const util::bezier<2> upright({p0, p1, p2});

    tap.expect_eq (upright.region (), util::region2f (p0, p2), "bezier-2 region");
}


//-----------------------------------------------------------------------------
template <>
void
test_region<3> (util::TAP::logger &tap)
{
    util::point2f p0 {   0,   0 },
                  p1 {  33,  33 },
                  p2 {  67,  67 },
                  p3 { 100, 100 };

    static const util::bezier<3> upright({p0, p1, p2, p3});

    tap.expect_eq (upright.region (), util::region2f (p0, p3), "bezier-3 region");
}


//-----------------------------------------------------------------------------
int
main (int, char**)
{
    util::TAP::logger tap;

    test_eval<1> (tap);
    test_eval<2> (tap);
    test_eval<3> (tap);

    test_intersect<1> (tap);
    test_intersect<2> (tap);
    test_intersect<3> (tap);

    test_region<1> (tap);
    test_region<2> (tap);
    test_region<3> (tap);

    return EXIT_SUCCESS;
}
