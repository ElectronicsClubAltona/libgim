#include "bezier.hpp"

#include "debug.hpp"

#include <cstdlib>


//-----------------------------------------------------------------------------
template <size_t> void test_eval (void);
template <size_t> void test_intersect (void);


//-----------------------------------------------------------------------------
template <>
void
test_eval<1> (void)
{
    static const util::bezier<1> b1 ({{  0.f,   0.f},
                                      {100.f, 100.f}});

    auto p0 = b1.eval(0.f);
    auto p1 = b1.eval(1.f);

    CHECK_EQ (p0, b1[0]);
    CHECK_EQ (p1, b1[1]);

    auto px = b1.eval(0.5f);
    auto rx = b1[0] + 0.5f * (b1[1]-b1[0]);
    CHECK_EQ (px, rx);
}


//-----------------------------------------------------------------------------
template <>
void
test_eval<2> (void)
{
    static const util::bezier<2> b2 ({{  0.f,   0.f},
                                      { 50.f,  50.f},
                                      {100.f, 100.f}});

    auto p0 = b2.eval(0.f);
    auto p2 = b2.eval(1.f);

    CHECK_EQ (p0, b2[0]);
    CHECK_EQ (p2, b2[2]);

    auto px = b2.eval(0.5f);
    auto rx = b2[0] + 0.5f * (b2[2]-b2[0]);
    CHECK_EQ (px, rx);
}


//-----------------------------------------------------------------------------
template <>
void
test_eval<3> (void)
{
    static const util::bezier<3> b3 ({{   0.f,   0.f },
                                      {  33.f,  33.f },
                                      {  67.f,  67.f },
                                      { 100.f, 100.f }});

    auto p0 = b3.eval (0.f);
    auto p3 = b3.eval (1.f);

    CHECK_EQ (p0, b3[0]);
    CHECK_EQ (p3, b3[3]);

    auto px = b3.eval (0.5f);
    auto rx = b3[0] + 0.5f * (b3[3] - b3[0]);
    CHECK_EQ (px, rx);
}


//-----------------------------------------------------------------------------
template <>
void
test_intersect<1> (void)
{
    // A line from (0,0) to (100,100)
    static const util::bezier<1> b1 ({{0.f, 0.f}, {100.f, 100.f}});

    // Through the centre
    CHECK_EQ (b1.intersections ({0.f, 100.f}, {100.f, 0.f}), 1);
    CHECK_EQ (b1.intersections ({100.f, 0.f}, {0.f, 100.f}), 1);

    // Coincident with endpoints
    CHECK_EQ (b1.intersections ({0.f, 0.f}, {1.f,0.f}), 1);
    CHECK_EQ (b1.intersections ({100.f, 100.f}, {100.f,0.f}), 1);

    // Co-planar
    CHECK_EQ (b1.intersections ({0.f, 0.f}, {1.f, 1.f}), 1);

    // Underneath
    CHECK_EQ (b1.intersections ({1000.f, -10.f}, {-1000.f, -10.f}), 0);

    // Above
    CHECK_EQ (b1.intersections ({1000.f, 110.f}, {-1000.f, 110.f}), 0);

}

//-----------------------------------------------------------------------------
template <>
void
test_intersect<2> (void)
{
    // A linear curve from (0,0) to (100,100)
    static const util::bezier<2> b2 ({{ 0.0f,  0.0f},
                                      { 50.f,  50.f},
                                      {100.f, 100.f}});

    // Through the centre
    CHECK_EQ (b2.intersections ({100.f, 0.f}, {0.f, 100.f}), 1);
    CHECK_EQ (b2.intersections ({0.f, 100.f}, {100.f, 0.f}), 1);

    // Coincident with endpoints
    CHECK_EQ (b2.intersections ({0.f, 0.f}, {0.f,100.f}), 1);
    CHECK_EQ (b2.intersections ({0.f, 0.f}, {100.f,0.f}), 1);
    CHECK_EQ (b2.intersections ({100.f, 100.f}, {100.f,0.f}), 1);

    // Co-planar
    CHECK_EQ (b2.intersections ({0.f, 0.f}, {1.f, 1.f}), 1);

    // Underneath
    CHECK_EQ (b2.intersections ({1000.f, -10.f}, {-1000.f, -10.f}), 0);

    // Above
    CHECK_EQ (b2.intersections ({1000.f, 110.f}, {-1000.f, 110.f}), 0);
}


//-----------------------------------------------------------------------------
template <>
void
test_intersect<3> (void)
{
    // A linear curve from (0,0) to (100,100)
    static const util::bezier<3> b3 ({{   0.f,   0.f },
                                      {  33.f,  33.f },
                                      {  67.f,  67.f },
                                      { 100.f, 100.f }});

    // Through the centre
    CHECK_EQ (b3.intersections ({100.f, 0.f}, {0.f, 100.f}), 1);
    CHECK_EQ (b3.intersections ({0.f, 100.f}, {100.f, 0.f}), 1);

    // Coincident with endpoints
    CHECK_EQ (b3.intersections ({0.f, 0.f}, {0.f,100.f}), 1);
    CHECK_EQ (b3.intersections ({0.f, 0.f}, {100.f,0.f}), 1);
    CHECK_EQ (b3.intersections ({100.f, 100.f}, {100.f,0.f}), 1);

    // Co-planar
    CHECK_EQ (b3.intersections ({0.f, 0.f}, {1.f, 1.f}), 1);

    // Underneath
    CHECK_EQ (b3.intersections ({1000.f, -10.f}, {-1000.f, -10.f}), 0);

    // Above
    CHECK_EQ (b3.intersections ({1000.f, 110.f}, {-1000.f, 110.f}), 0);
}


//-----------------------------------------------------------------------------
int
main (int, char**)
{
    test_eval<1> ();
    test_eval<2> ();
    test_eval<3> ();

    test_intersect<1> ();
    test_intersect<2> ();
    test_intersect<3> ();

    return EXIT_SUCCESS;
}
