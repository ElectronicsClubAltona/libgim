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

    // Check lookups are working
    CHECK_EQ (util::colour4f::from_html ("white"), util::colour4f::WHITE);
    CHECK_EQ (util::colour4f::from_x11  ("white"), util::colour4f::WHITE);

    // Check HSV conversions
    {
        // white: hue is undefined
        auto white = util::rgb_to_hsv ({1,1,1});
        CHECK_EQ (white.s, 0);
        CHECK_EQ (white.v, 1);

        // black: hue is undefined
        auto black = util::rgb_to_hsv ({0,0,0});
        CHECK_EQ (black.s, 0);
        CHECK_EQ (black.v, 0);

        struct {
            util::colour3f rgb;
            util::colour3f hsv;
        } TESTS[] = {
            { { 1,     0,     0,    }, {   0, 1,     1,    } }, // red
            { { 0,     1,     0,    }, { 120, 1,     1,    } }, // green
            { { 0,     0,     1,    }, { 240, 1,     1,    } }, // blue
            { { 0.75f, 0.25f, 0.75f }, { 300, 2/3.f, 0.75f } },
        };

        for (auto i: TESTS) {
            CHECK_EQ (util::rgb_to_hsv (i.rgb), i.hsv);
            CHECK_EQ (util::hsv_to_rgb (i.hsv), i.rgb);
        }
    }
}
