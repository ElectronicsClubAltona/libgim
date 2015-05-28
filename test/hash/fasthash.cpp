#include "hash/fasthash.hpp"
#include "tap.hpp"

#include <cstring>

int
main (void)
{
    util::TAP::logger tap;

    static const struct {
        uint32_t seed32;
        uint32_t hash32;
        uint64_t seed64;
        uint64_t hash64;
        const char *str;
    } TESTS[] = {
        { 0x00000000, 0x00000000, 0x0000000000000000, 0x0000000000000000, "" },
        { 0x00000001, 0xd30ac4de, 0x0000000000000001, 0x2127599bf4321e79, "" },
        { 0xffffffff, 0xf5c7b4b0, 0xffffffffffffffff, 0x9b4792000001368f, "" },
        { 0xf5c7b4b0, 0x228128b7, 0x9b4792000001368f, 0x67a642098cc81da6, "a" },
        { 0x228128b7, 0x8400568d, 0x67a642098cc81da6, 0xc906440e03ce99a8, "abc" },
        { 0x8400568d, 0x12b4858b, 0x67a642098cc81da6, 0x1a36fbf3d71b0737, "message digest" },
        { 0x12b4858b, 0x730b822e, 0x1a36fbf3d71b0737, 0x7b48e31e3ac40a0f, "abcdefghijklmnopqrstuvwxyz" },
    };

    bool success32 = true;
    bool success64 = true;

    for (const auto &t: TESTS) {
        success32 = success32 && t.hash32 == util::hash::fasthash::hash32 (t.str, strlen (t.str), t.seed32);
        success64 = success64 && t.hash64 == util::hash::fasthash::hash64 (t.str, strlen (t.str), t.seed64);
    }

    tap.expect (success32, "fasthash32");
    tap.expect (success64, "fasthash64");

    return tap.status ();
}
