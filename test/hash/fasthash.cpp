#include "hash/fasthash.hpp"
#include "tap.hpp"

#include <cstring>


///////////////////////////////////////////////////////////////////////////////
std::vector<uint8_t>
operator"" _u8s (const char *str, size_t len)
{
    std::vector<uint8_t> res;
    res.resize (len);
    std::copy_n (str, len, std::begin (res));
    return res;
}


///////////////////////////////////////////////////////////////////////////////
int
main (void)
{
    util::TAP::logger tap;

    static const struct {
        uint32_t seed32;
        uint32_t hash32;
        uint64_t seed64;
        uint64_t hash64;
        std::vector<uint8_t> data;
    } TESTS[] = {
        { 0x00000000, 0x00000000, 0x0000000000000000, 0x0000000000000000, ""_u8s },
        { 0x00000001, 0xd30ac4de, 0x0000000000000001, 0x2127599bf4321e79, ""_u8s },
        { 0xffffffff, 0xf5c7b4b0, 0xffffffffffffffff, 0x9b4792000001368f, ""_u8s },
        { 0xf5c7b4b0, 0x228128b7, 0x9b4792000001368f, 0x67a642098cc81da6, "a"_u8s },
        { 0x228128b7, 0x8400568d, 0x67a642098cc81da6, 0xc906440e03ce99a8, "abc"_u8s },
        { 0x8400568d, 0x12b4858b, 0x67a642098cc81da6, 0x1a36fbf3d71b0737, "message digest"_u8s },
        { 0x12b4858b, 0x730b822e, 0x1a36fbf3d71b0737, 0x7b48e31e3ac40a0f, "abcdefghijklmnopqrstuvwxyz"_u8s },
    };

    bool success32 = true;
    bool success64 = true;

    util::hash::fasthash<uint32_t> h32{};
    util::hash::fasthash<uint64_t> h64{};

    for (const auto &t: TESTS) {
        success32 = success32 && t.hash32 == h32 (t.seed32, t.data);
        success64 = success64 && t.hash64 == h64 (t.seed64, t.data);
    }

    tap.expect (success32, "fasthash32");
    tap.expect (success64, "fasthash64");

    return tap.status ();
}
