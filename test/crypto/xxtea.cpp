#include "crypto/xxtea.hpp"

#include "tap.hpp"
#include "types.hpp"


int
main ()
{
    // test vectors from 'TeaCrypt', by Logan J. Drews.
    static const struct {
        std::array<uint32_t,4> key;
        std::vector<uint32_t> dec;
        std::vector<uint32_t> enc;
    } TESTS[] = {
        // 64 bit
        {
            { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
            { 0x00000000, 0x00000000 },
            { 0x053704AB, 0x575D8C80 }
        },

        {
            {0x00000000, 0x00000000, 0x00000000, 0x00000000},
            {0x01020304, 0x05060708},
            {0xE6911910, 0x0C35DCDA},
        },
          
        {
            {0x00112233, 0x44556677, 0x8899AABB, 0xCCDDEEFF},
            {0x01020304, 0x05060708},
            {0x961D49FC, 0x61FF12D6},
        },

        {            
            {0x00112233, 0x44556677, 0x8899AABB, 0xCCDDEEFF},
            {0x01234567, 0x89ABCDEF},
            {0x34354989, 0xDD7D1A7A},
        },

        // 96 bit
        {
            {0x00000000, 0x00000000, 0x00000000, 0x00000000},
            {0x00000000, 0x00000000, 0x00000000},
            {0x5E3CD3F0, 0xE109E3CE, 0x79D7C945},
        },

        {
            {0x00000000, 0x00000000, 0x00000000, 0x00000000},
            {0x00010203, 0x04050607, 0x08090A0B},
            {0x5A545AAC, 0x684EB2CB, 0x3E1B8AA0},
        },

        {
            {0x00112233, 0x44556677, 0x8899AABB, 0xCCDDEEFF},
            {0x00010203, 0x04050607, 0x08090A0B},
            {0x2E77CCEC, 0x674F5149, 0xA0E56496},
        },

        {
            {0x00112233, 0x44556677, 0x8899AABB, 0xCCDDEEFF},
            {0x01234567, 0x89ABCDEF, 0x01234567},
            {0xEBC5DD46, 0xBE0FEE71, 0xC6BF7193},
        },

        // 128 bit
        {
            {0x00000000, 0x00000000, 0x00000000, 0x00000000},
            {0x00000000, 0x00000000, 0x00000000, 0x00000000},
            {0xE6C8D5FF, 0x070FB6E4, 0x98A534F7, 0xAC03E399},
        },

        {
            {0x00000000, 0x00000000, 0x00000000, 0x00000000},
            {0x00010203, 0x04050607, 0x08090A0B, 0x0C0D0E0F},
            {0xAF5CFB0E, 0xAE73552B, 0x1D968A9F, 0x5CB94509},
        },

        {
            {0x00112233, 0x44556677, 0x8899AABB, 0xCCDDEEFF},
            {0x00010203, 0x04050607, 0x08090A0B, 0x0C0D0E0F},
            {0x3EA0E16C, 0x9969535A, 0xE4796D50, 0xF217EEEA},
        },

        {
            {0x00112233, 0x44556677, 0x8899AABB, 0xCCDDEEFF},
            {0x01234567, 0x89ABCDEF, 0x01234567, 0x89ABCDEF},
            {0x2B4AB1A4, 0x0E487B6D, 0x9A3AACC7, 0xE4132216},
        },
    };

    util::TAP::logger tap;

    for (size_t i = 0; i < elems (TESTS); ++i) {
        const auto &t = TESTS[i];

        CHECK_EQ (t.dec.size (), t.enc.size ());

        util::crypto::XXTEA gen (t.key);

        std::vector<uint32_t> enc (t.dec);
        gen.encrypt (enc.data (), enc.size ());

        std::vector<uint32_t> dec (enc);
        gen.decrypt (dec.data (), dec.size ());

        tap.expect (enc == t.enc, "XXTEA_enc %zu", i);
        tap.expect (dec == t.dec, "XXTEA_dec %zu", i);
    }

    return tap.status ();
}
