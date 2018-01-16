#include "tap.hpp"
#include "hash/crc.hpp"

#include <cstdint>
#include <utility>
#include <vector>


///////////////////////////////////////////////////////////////////////////////
static const
struct {
    struct {
        uint32_t crc32;
        uint32_t crc32b;
        uint32_t crc32c;
        uint32_t crc32d;
        uint64_t crc64;
    } result;
    const char *dat;
    const char *msg;
} TESTS[] = {
    { { 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000000000000000 }, "",  "empty" },
    { { 0xe8b7be43, 0x19939b6b, 0xc1d04330, 0x60819b69, 0x548f120162451c62 }, "a", "single letter" },
    { { 0xa684c7c6, 0x96b0e4e0, 0x280c069e, 0x95ed974e, 0x2a71ab4164c3bbe8 }, "0123456789", "10 digits" },
    { { 0x414fa339, 0x459dee61, 0x22620404, 0x9d251c62, 0x41e05242ffa9883b }, "The quick brown fox jumps over the lazy dog", "quick brown fox" },
};


///////////////////////////////////////////////////////////////////////////////
int
main (int, char**)
{
    util::TAP::logger tap;

    for (const auto &t: TESTS) {
        #define TEST(KLASS) do { \
            auto computed = util::hash::KLASS{}(util::view {t.dat}.template cast<const uint8_t> ()); \
            tap.expect_eq (t.result.KLASS, computed, "%s: %s", #KLASS, t.msg); \
        } while (0)

        TEST(crc32);
        TEST(crc32b);
        TEST(crc32c);
        TEST(crc32d);
        TEST(crc64);
    }

    return tap.status ();
}