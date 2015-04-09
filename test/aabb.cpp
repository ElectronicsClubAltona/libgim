#include "aabb.hpp"

int
main (int, char**)
{
    {
        // Test contraction
        util::AABB2f box {
            { 2, 2 },
            { 8, 8 }
        };

        box.contract (2.f);

        CHECK_EQ (box.p0.x, 3);
        CHECK_EQ (box.p0.y, 3);
        CHECK_EQ (box.p1.x, 7);
        CHECK_EQ (box.p1.y, 7);
    }

    {
        // Test expansion
        util::AABB2f box {
            { 2, 2 },
            { 8, 8 }
        };

        box.expand (2.f);
        
        CHECK_EQ (box.p0.x, 1);
        CHECK_EQ (box.p0.y, 1);
        CHECK_EQ (box.p1.x, 9);
        CHECK_EQ (box.p1.y, 9);
    }


    {
        // Ensure we don't wrap-around on unsigned position types when contracting
        util::AABB2f small {
            { 0, 0 },
            { 1, 1 }
        };

        small.contract (10);

        CHECK_EQ (small.p0.x, 0.5f);
        CHECK_EQ (small.p0.y, 0.5f);
        CHECK_EQ (small.p1.x, 0.5f);
        CHECK_EQ (small.p1.y, 0.5f);
    }
}
