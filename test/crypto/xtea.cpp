#include "tap.hpp"
#include "crypto/xtea.hpp"
#include "types.hpp"

int
main ()
{
    // test vectors from 'TeaCrypt', by Logan J. Drews.
    static const struct {
        std::array<uint32_t,4> key;
        std::array<uint32_t,2> dec;
        std::array<uint32_t,2> enc;
    } TESTS[] = {
        {
            {0x00000000, 0x00000000, 0x00000000, 0x00000000},
            {0x00000000, 0x00000000},
            {0xDEE9D4D8, 0xF7131ED9},
        },

        {
            {0x00000000, 0x00000000, 0x00000000, 0x00000000},
            {0x01020304, 0x05060708},
            {0x065C1B89, 0x75C6A816},
        },

        {
            {0x00112233, 0x44556677, 0x8899AABB, 0xCCDDEEFF},
            {0x01020304, 0x05060708},
            {0xDCDD7ACD, 0xC1584B79},

        },

        {
            {0x00112233, 0x44556677, 0x8899AABB, 0xCCDDEEFF},
            {0x01234567, 0x89ABCDEF},
            {0xB8BF2821, 0x622B5B30},
        },
    };

    util::TAP::logger tap;

    for (size_t i = 0; i < elems (TESTS); ++i) {
        const auto &t = TESTS[i];
        util::crypto::XTEA gen (t.key);

        std::array<uint32_t,2> enc (t.dec);
        gen.encrypt (enc.data (), enc.size ());

        std::array<uint32_t,2> dec (t.enc);
        gen.decrypt (dec.data (), dec.size ());

        tap.expect (enc == t.enc, "XTEA_enc %zu", i);
        tap.expect (dec == t.dec, "XTEA_dec %zu", i);
    }

    return tap.status ();
}
