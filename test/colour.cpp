#include "colour.hpp"
 
#include "debug.hpp"

int
main (int, char**)
{
    // Simple check for symbol visibility
    CHECK_EQ (util::colour4f::WHITE.r, 1.f);
    CHECK_EQ (util::colour4f::WHITE.g, 1.f);
    CHECK_EQ (util::colour4f::WHITE.b, 1.f);
    CHECK_EQ (util::colour4f::WHITE.a, 1.f);

    // Check casting works between intergral and floating formats
    {
        util::colour4f f (1);
        util::colour<4,uint8_t> u (255);
        CHECK_EQ (f.cast<uint8_t> (), u);
        CHECK_EQ (u.cast<float> (), f);
    }
}
