#include "colour.hpp"

#include "tap.hpp"


int
main (int, char**)
{
    util::TAP::logger tap;

    // Check casting works between intergral and floating formats
    {
        util::srgba<4,float> f {1.f};
        util::srgba<4,uint8_t> u {255};
        tap.expect_eq (f.cast<uint8_t> (), u, "cast float to u8");
        tap.expect_eq (u.cast<float> (), f, "cast u8 to float");
    }

    return tap.status ();
}
