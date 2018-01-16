#include "tap.hpp"

#include "hash/murmur.hpp"
#include "types.hpp"

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
template <size_t N>
std::ostream&
operator<< (std::ostream &os, std::array<uint8_t,N> &val)
{
    for (auto c: val)
        os << c;
    return os;
}


///////////////////////////////////////////////////////////////////////////////
void
test (util::TAP::logger &tap)
{
    struct {
        std::vector<uint8_t> data;
        const char *msg;

        struct { uint32_t seed; uint32_t hash; } m1_32;
        struct { uint32_t seed; uint32_t hash; } m2_32;
        struct { uint64_t seed; uint64_t hash; } m2_64;
        struct { uint32_t seed; uint32_t hash; } m3_32;

        struct { uint32_t seed; std::array<uint32_t,4> hash; } m3_128_x86;
        struct { uint32_t seed; std::array<uint64_t,2> hash; } m3_128_x64;

    } TESTS[] = {
        { ""_u8s,
          "empty, zero seed",
            { 0, 0 },
            { 0, 0 },
            { 0, 0 },
            { 0, 0 },
            { 0, 0 },
            { 0, 0 },
        },

        { ""_u8s,
          "empty, nonzero seed",
            { 0x00000001, 0x8f5a8d63 },
            { 0x00000001, 0x5bd15e36 },
            {          1, 0xc6a4a7935bd064dc },
            { 1, 0x514e28b7 },
            { 1, { 0x88c4adec, 0x54d201b9, 0x54d201b9, 0x54d201b9 } },
            { 1, { 0x4610abe56eff5cb5, 0x51622daa78f83583 } },
        },

        { ""_u8s,
          "empty, max seed",
            { 0xffffffff, 0x7a3f4f7e },
            { 0xffffffff, 0xb35966b0 },
            { uint64_t(-1), 0xb0d9485c2cd761b2 },
            { 0xffffffff, 0x81f16f39 },
            { 0xffffffff, { 0x051e08a9, 0x989d49f7, 0x989d49f7, 0x989d49f7 } },
            { 0xffffffff, { 0x6af1df4d9d3bc9ec, 0x857421121ee6446b } },
        },

        { "a"_u8s,
          "1 byte",
            { 0x7a3f4f7e, 0x18abad09 },
            { 0xb35966b0, 0x1eea8b10 },
            { 0xb0d9485c2cd761b2, 0x0a9b4c93b35b1b9f },
            { 0x81f16f39, 0x80d3460d },
            { 0x051e08a9, { 0x08e91d27, 0x12c6d92a, 0x12c6d92a, 0x12c6d92a } },
            { 0x9d3bc9ec, { 0xf79489c9f1a785de, 0xf6486d31835a9c7f } },
        },
        { "abc"_u8s,
          "3 byte",
            { 0x18abad09, 0x1defb5e9 },
            { 0x1eea8b10, 0x72cac527 },
            { 0x0a9b4c93b35b1b9f, 0x2ffdf3214d9a4fa4 },
            { 0x80d3460d, 0x622f3384 },
            { 0x08e91d27, { 0xc11cc883, 0xb5d7f69a, 0xb5d7f69a, 0xb5d7f69a } },
            { 0xf1a785de, { 0x946e5ee63ce3b80e, 0xadb7d6d0e2558c3c } },
        },
        { "message digest"_u8s,
          "14 byte",
            { 0x1defb5e9, 0x7b3ea4bd },
            { 0x72cac527, 0x68563c37 },
            { 0x2ffdf3214d9a4fa4, 0x9a83e79336350cee },
            { 0x622f3384, 0x6884feac },
            { 0xc11cc883, { 0x9e3cc608, 0x90eba4c0, 0xc3775cf8, 0x10eb4fee } },
            { 0x3ce3b80e, { 0x2c91b16326bf5f7f, 0xa21acf13c39485bc } },
        },

        { "abcdefghijklmnopqrstuvwxyz"_u8s,
          "26 byte",
            { 0x7b3ea4bd, 0xd94ee9ea },
            { 0x68563c37, 0x0473b699 },
            { 0x9a83e79336350cee, 0x1f256c898952ae12 },
            { 0x6884feac, 0x99befd9e },
            { 0x9e3cc608, { 0x7de6cb3c, 0x62268725, 0x7c9f0b06, 0x5d7e2281 } },
            { 0x26bf5f7f, { 0xdbe75dee9ce0b3cb, 0x006c66d16f72b399, } },
        }
    };

    for (const auto &t: TESTS) {
        const util::hash::murmur1 h1 (t.m1_32.seed);

        const util::hash::murmur2<uint32_t> h2_32 (t.m2_32.seed);
        const util::hash::murmur2<uint64_t> h2_64 (t.m2_64.seed);

        const util::hash::murmur3_32 h3 (t.m3_32.seed);
        const util::hash::murmur3_128_x86 h3_x86 (t.m3_128_x86.seed);
        const util::hash::murmur3_128_x64 h3_x64 (t.m3_128_x64.seed);

        tap.expect_eq (h1 (t.data), t.m1_32.hash, "murmur1_32, '%s'", t.msg);
        tap.expect_eq (h2_32 (t.data), t.m2_32.hash, "murmur2_32, '%s'", t.msg);
        tap.expect_eq (h2_64 (t.data), t.m2_64.hash, "murmur2_64, '%s'", t.msg);

        tap.expect_eq (h3 (t.data), t.m3_32.hash, "murmur3_32, '%s'", t.msg);
        tap.expect_eq (h3_x86 (t.data), t.m3_128_x86.hash, "murmur3_128_x86, '%s'", t.msg);
        tap.expect_eq (h3_x64 (t.data), t.m3_128_x64.hash, "murmur3_128_x64, '%s'", t.msg);
    }
}



///////////////////////////////////////////////////////////////////////////////
int
main (void)
{
    util::TAP::logger tap;

    test (tap);

    return tap.status ();
}
