#include "tap.hpp"
#include "types.hpp"
#include "crypto/tea.hpp"


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
            { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
            { 0x00000000, 0x00000000 },
            { 0x41EA3A0A, 0x94BAA940 },
        },

        {
            { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
            { 0x01020304, 0x05060708 },
            { 0x6A2F9CF3, 0xFCCF3C55 },
        },

        {
            { 0x00112233, 0x44556677, 0x8899AABB, 0xCCDDEEFF },
            { 0x01020304, 0x05060708 },
            { 0xDEB1C0A2, 0x7E745DB3 },
        },

        {
            { 0x00112233, 0x44556677, 0x8899AABB, 0xCCDDEEFF },
            { 0x01234567, 0x89ABCDEF },
            { 0x126C6B92, 0xC0653A3E },
        },
    };

    util::TAP::logger tap;

    for (size_t i = 0; i < elems (TESTS); ++i) {
        const auto &t = TESTS[i];
        util::crypto::TEA gen (t.key);

        std::array<uint32_t,2> enc (t.dec);
        gen.encrypt (enc.data (), enc.size ());

        std::array<uint32_t,2> dec (t.enc);
        gen.decrypt (dec.data (), dec.size ());
        
        {
            std::ostringstream os;
            os << "TEA_enc " << i;
            tap.expect (enc == t.enc, os.str ());
        }

        {
            std::ostringstream os;
            os << "TEA_dec " << i;
            tap.expect (dec == t.dec, os.str ());
        }
    }

    return tap.status ();
}
