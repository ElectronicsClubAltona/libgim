#include "../bitwise.hpp"

int
main (int, char**) {
    CHECK_EQ (rotatel (uint8_t (0x0F), 0), 0x0F);
    CHECK_EQ (rotatel (uint8_t (0x0F), 4), 0xF0);
    CHECK_EQ (rotatel (uint8_t (0xF0), 4), 0x0F);
    CHECK_EQ (rotatel (uint8_t (0x0F), 8), 0x0F);

    CHECK_EQ (rotater (uint8_t (0x0F), 0), 0x0F);
    CHECK_EQ (rotater (uint8_t (0x0F), 4), 0xF0);
    CHECK_EQ (rotater (uint8_t (0xF0), 4), 0x0F);
    CHECK_EQ (rotater (uint8_t (0x0F), 8), 0x0F);

    CHECK_EQ (rotatel (uint16_t (0xABCD),  0), 0xABCD);
    CHECK_EQ (rotatel (uint16_t (0xABCD),  4), 0xBCDA);
    CHECK_EQ (rotatel (uint16_t (0xABCD),  8), 0xCDAB);
    CHECK_EQ (rotatel (uint16_t (0xABCD), 12), 0xDABC);
    CHECK_EQ (rotatel (uint16_t (0xABCD), 16), 0xABCD);

    CHECK_EQ (rotater (uint16_t (0xABCD),  0), 0xABCD);
    CHECK_EQ (rotater (uint16_t (0xABCD),  4), 0xDABC);
    CHECK_EQ (rotater (uint16_t (0xABCD),  8), 0xCDAB);
    CHECK_EQ (rotater (uint16_t (0xABCD), 12), 0xBCDA);
    CHECK_EQ (rotater (uint16_t (0xABCD), 16), 0xABCD);

    CHECK_EQ (rotatel (uint32_t (0x12345670), 12), 0x45670123);
    CHECK_EQ (rotater (uint32_t (0x12345670), 12), 0x67012345);

    CHECK_EQ (rotatel (uint64_t (0x1234567890ABCDEF), 12), 0x4567890ABCDEF123);
    CHECK_EQ (rotater (uint64_t (0x1234567890ABCDEF), 12), 0xDEF1234567890ABC);

    return 0;
}
