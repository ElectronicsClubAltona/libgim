#include "bezier.hpp"

#include "debug.hpp"

#include <cstdlib>

int
main (int, char**)
{
    // Check degree-1 beziers
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

    // Check degree-2 beziers
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

    // Check degree-3 beziers
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

    return EXIT_SUCCESS;
}
