#include "colour.hpp"

#include "tap.hpp"


int
main (int, char**)
{
    util::TAP::logger tap;

    // Check casting works between intergral and floating formats
    {
        util::colour4f f (1);
        util::colour<4,uint8_t> u (255);
        tap.expect_eq (f.cast<uint8_t> (), u, "cast float to u8");
        tap.expect_eq (u.cast<float> (), f, "cast u8 to float");
    }

    // Check parsing is working
    tap.expect_eq (
        util::colour4u::parse_html ("#11223344"),
        util::colour4u (0x11, 0x22, 0x33, 0x44),
        "4-component html parsing with hash"
    );

    tap.expect_eq (
        util::colour4u::parse_html ("11223344"),
        util::colour4u (0x11, 0x22, 0x33, 0x44),
        "4-component html parsing without hash"
    );

    tap.expect_eq (
        util::colour1f::parse_html ("ff"),
        util::colour1f (1.f),
        "1-component html parsing"
    );

    tap.expect_eq (
        util::colour3u::parse_html ("3399ff"),
        util::colour3u (0x33, 0x99, 0xff),
        "3-component html parsing"
    );

    tap.expect_throw<std::invalid_argument> (
        [] () { util::colour1f::parse_html ("00112233"); },
        "1-component parsing with 4-component format"
    );

    tap.expect_throw<std::invalid_argument> (
        [] () { util::colour4f::parse_html ("00"); },
        "4-component parsing with 1-component format"
    );

    tap.expect_throw<std::invalid_argument> (
        [] () { util::colour4f::parse_html ("0011223"); },
        "4-component parsing with one too few digits"
    );


    // Check lookups are working
    tap.expect_eq (util::colour4f::from_html  ("white"), util::colour4f {1}, "HTML lookup");
    tap.expect_eq ( util::colour4f::from_x11  ("white"), util::colour4f {1}, "X11 lookup");

    // Check HSV conversions
    {
        // white: hue is undefined
        auto white = util::rgb_to_hsv ({1,1,1});
        tap.expect (util::exactly_zero (white.s) && util::exactly_equal (white.v, 1), "white hsv");

        // black: hue is undefined
        auto black = util::rgb_to_hsv ({0,0,0});
        tap.expect (util::exactly_zero (black.s) && util::exactly_zero (black.v), "black hsv");

        struct {
            const char *name;
            util::colour3f rgb;
            util::colour3f hsv;
        } TESTS[] = {
            { "red",   { 1,     0,     0,    }, {   0, 1,     1,    } }, // red
            { "green", { 0,     1,     0,    }, { 120, 1,     1,    } }, // green
            { "blue",  { 0,     0,     1,    }, { 240, 1,     1,    } }, // blue
            { "misc",  { 0.75f, 0.25f, 0.75f }, { 300, 2/3.f, 0.75f } },
        };

        for (auto i: TESTS) {
            tap.expect_eq (util::rgb_to_hsv (i.rgb), i.hsv, "rgb-to-hsv %s", i.name);
            tap.expect_eq (util::hsv_to_rgb (i.hsv), i.rgb, "hsv-to-rgb %s", i.name);
        }
    }

    return tap.status ();
}
