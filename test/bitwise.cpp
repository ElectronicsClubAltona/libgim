#include "bitwise.hpp"
#include "tap.hpp"


//-----------------------------------------------------------------------------
static void
test_rotate (util::TAP::logger &tap)
{
    tap.expect_eq (rotatel (uint8_t (0x0F), 0), 0x0F, "rotate-left u8");
    tap.expect_eq (rotatel (uint8_t (0x0F), 4), 0xF0, "rotate-left u8");
    tap.expect_eq (rotatel (uint8_t (0xF0), 4), 0x0F, "rotate-left u8");
    tap.expect_eq (rotatel (uint8_t (0x0F), 8), 0x0F, "rotate-left u8");

    tap.expect_eq (rotater (uint8_t (0x0F), 0), 0x0F, "rotate-right u8");
    tap.expect_eq (rotater (uint8_t (0x0F), 4), 0xF0, "rotate-right u8");
    tap.expect_eq (rotater (uint8_t (0xF0), 4), 0x0F, "rotate-right u8");
    tap.expect_eq (rotater (uint8_t (0x0F), 8), 0x0F, "rotate-right u8");

    tap.expect_eq (rotatel (uint16_t (0xABCD),  0), 0xABCD, "rotate-left u16");
    tap.expect_eq (rotatel (uint16_t (0xABCD),  4), 0xBCDA, "rotate-left u16");
    tap.expect_eq (rotatel (uint16_t (0xABCD),  8), 0xCDAB, "rotate-left u16");
    tap.expect_eq (rotatel (uint16_t (0xABCD), 12), 0xDABC, "rotate-left u16");
    tap.expect_eq (rotatel (uint16_t (0xABCD), 16), 0xABCD, "rotate-left u16");

    tap.expect_eq (rotater (uint16_t (0xABCD),  0), 0xABCD, "rotate-right u16");
    tap.expect_eq (rotater (uint16_t (0xABCD),  4), 0xDABC, "rotate-right u16");
    tap.expect_eq (rotater (uint16_t (0xABCD),  8), 0xCDAB, "rotate-right u16");
    tap.expect_eq (rotater (uint16_t (0xABCD), 12), 0xBCDA, "rotate-right u16");
    tap.expect_eq (rotater (uint16_t (0xABCD), 16), 0xABCD, "rotate-right u16");

    tap.expect_eq (rotatel (uint32_t (0x12345670), 12), 0x45670123, "rotate-left u32");
    tap.expect_eq (rotater (uint32_t (0x12345670), 12), 0x67012345, "rotate-right u32");

    tap.expect_eq (rotatel (uint64_t (0x1234567890ABCDEF), 12), 0x4567890ABCDEF123, "rotate-left u64");
    tap.expect_eq (rotater (uint64_t (0x1234567890ABCDEF), 12), 0xDEF1234567890ABC, "rotate-right u64");
}


//-----------------------------------------------------------------------------
void
test_reverse (util::TAP::logger &tap)
{
    size_t matches = 0;
    for (unsigned i = 0; i < 256; ++i) {
        auto first = reverse<uint8_t> (i);
        auto last  = reverse<uint8_t> (first);
        if (last == i)
            matches++;
    }

    tap.expect_eq (matches, 256, "exhaustive 8 bit reverse");
}


//-----------------------------------------------------------------------------
int
main (int, char**)
{
    util::TAP::logger tap;

    test_rotate (tap);
    test_reverse (tap);

    return 0;
}
