#include "bitwise.hpp"
#include "tap.hpp"


//-----------------------------------------------------------------------------
static void
test_rotate (util::TAP::logger &tap)
{
    tap.expect_eq (util::rotatel (uint8_t (0x0F), 0), 0x0Fu, "rotate-left u8");
    tap.expect_eq (util::rotatel (uint8_t (0x0F), 4), 0xF0u, "rotate-left u8");
    tap.expect_eq (util::rotatel (uint8_t (0xF0), 4), 0x0Fu, "rotate-left u8");
    tap.expect_eq (util::rotatel (uint8_t (0x0F), 8), 0x0Fu, "rotate-left u8");

    tap.expect_eq (util::rotater (uint8_t (0x0F), 0), 0x0Fu, "rotate-right u8");
    tap.expect_eq (util::rotater (uint8_t (0x0F), 4), 0xF0u, "rotate-right u8");
    tap.expect_eq (util::rotater (uint8_t (0xF0), 4), 0x0Fu, "rotate-right u8");
    tap.expect_eq (util::rotater (uint8_t (0x0F), 8), 0x0Fu, "rotate-right u8");

    tap.expect_eq (util::rotatel (uint16_t (0xABCD),  0), 0xABCDu, "rotate-left u16");
    tap.expect_eq (util::rotatel (uint16_t (0xABCD),  4), 0xBCDAu, "rotate-left u16");
    tap.expect_eq (util::rotatel (uint16_t (0xABCD),  8), 0xCDABu, "rotate-left u16");
    tap.expect_eq (util::rotatel (uint16_t (0xABCD), 12), 0xDABCu, "rotate-left u16");
    tap.expect_eq (util::rotatel (uint16_t (0xABCD), 16), 0xABCDu, "rotate-left u16");

    tap.expect_eq (util::rotater (uint16_t (0xABCD),  0), 0xABCDu, "rotate-right u16");
    tap.expect_eq (util::rotater (uint16_t (0xABCD),  4), 0xDABCu, "rotate-right u16");
    tap.expect_eq (util::rotater (uint16_t (0xABCD),  8), 0xCDABu, "rotate-right u16");
    tap.expect_eq (util::rotater (uint16_t (0xABCD), 12), 0xBCDAu, "rotate-right u16");
    tap.expect_eq (util::rotater (uint16_t (0xABCD), 16), 0xABCDu, "rotate-right u16");

    tap.expect_eq (util::rotatel (uint32_t (0x12345670), 12), 0x45670123u, "rotate-left u32");
    tap.expect_eq (util::rotater (uint32_t (0x12345670), 12), 0x67012345u, "rotate-right u32");

    tap.expect_eq (util::rotatel (uint64_t (0x1234567890ABCDEF), 12), 0x4567890ABCDEF123u, "rotate-left u64");
    tap.expect_eq (util::rotater (uint64_t (0x1234567890ABCDEF), 12), 0xDEF1234567890ABCu, "rotate-right u64");
}


//-----------------------------------------------------------------------------
void
test_reverse (util::TAP::logger &tap)
{
    size_t matches = 0;
    for (unsigned i = 0; i < 256; ++i) {
        auto first = util::reverse<uint8_t> (i);
        auto last  = util::reverse<uint8_t> (first);
        if (last == i)
            matches++;
    }

    tap.expect_eq (matches, 256u, "exhaustive 8 bit reverse");
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
