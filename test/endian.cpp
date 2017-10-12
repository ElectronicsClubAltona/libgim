#include "tap.hpp"

#include "endian.hpp"


///////////////////////////////////////////////////////////////////////////////
int
main (void)
{
    util::TAP::logger tap;

    {
        uint32_t a = 0x12345678, b = 0x78563412;
        tap.expect_eq (a, util::bswap (b), "u32 byteswap");
    }

    {
        // try to byte swap the pattern for 1.0f
        //
        // it may not be the most robust test, but it'll catch the most
        // egregious errors for the time being.
        union {
            uint32_t u;
            float f;
        } data { .u = 0x0000803f /* 0x3f800000 == 1.f */ };

        tap.expect_eq (util::bswap (data.f), 1.f, "f32 byteswap");
    };

    return tap.status ();
}