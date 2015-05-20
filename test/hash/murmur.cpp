#include "tap.hpp"

#include "hash/murmur.hpp"
#include "types.hpp"

#include <cstring>


void
test (util::TAP::logger &tap)
{
    struct {
        const char *key;

        struct { uint32_t seed; uint32_t hash; } m1_32;
        struct { uint32_t seed; uint32_t hash; } m2_32;
        struct { uint64_t seed; uint64_t hash; } m2_64;
        struct { uint32_t seed; uint32_t hash; } m3_32;

        struct { uint32_t seed; std::array<uint32_t,4> hash; } m3_128_x86;
        struct { uint32_t seed; std::array<uint64_t,2> hash; } m3_128_x64;

    } TESTS[] = {
        { "",
            { 0, 0 },
            { 0, 0 },
            { 0, 0 },
            { 0, 0 },
            { 0, 0 },
            { 0, 0 },
        },

        { "",
            { 0x00000001, 0x8f5a8d63 },
            { 0x00000001, 0x5bd15e36 },
            {          1, 0xc6a4a7935bd064dc },
            { 1, 0x514e28b7 },
            { 1, { 0x88c4adec, 0x54d201b9, 0x54d201b9, 0x54d201b9 } },
            { 1, { 0x4610abe56eff5cb5, 0x51622daa78f83583 } },
        },

        { "",
            { 0xffffffff, 0x7a3f4f7e },
            { 0xffffffff, 0xb35966b0 },
            { uint64_t(-1), 0xb0d9485c2cd761b2 },
            { 0xffffffff, 0x81f16f39 },
            { 0xffffffff, { 0x051e08a9, 0x989d49f7, 0x989d49f7, 0x989d49f7 } },
            { 0xffffffff, { 0x6af1df4d9d3bc9ec, 0x857421121ee6446b } },
        },

        { "a",
            { 0x7a3f4f7e, 0x18abad09 },
            { 0xb35966b0, 0x1eea8b10 },
            { 0xb0d9485c2cd761b2, 0x0a9b4c93b35b1b9f },
            { 0x81f16f39, 0x80d3460d },
            { 0x051e08a9, { 0x08e91d27, 0x12c6d92a, 0x12c6d92a, 0x12c6d92a } },
            { 0x9d3bc9ec, { 0xf79489c9f1a785de, 0xf6486d31835a9c7f } },
        },
        { "abc",
            { 0x18abad09, 0x1defb5e9 },
            { 0x1eea8b10, 0x72cac527 },
            { 0x0a9b4c93b35b1b9f, 0x2ffdf3214d9a4fa4 },
            { 0x80d3460d, 0x622f3384 },
            { 0x08e91d27, { 0xc11cc883, 0xb5d7f69a, 0xb5d7f69a, 0xb5d7f69a } },
            { 0xf1a785de, { 0x946e5ee63ce3b80e, 0xadb7d6d0e2558c3c } },
        },
        { "message digest",
            { 0x1defb5e9, 0x7b3ea4bd },
            { 0x72cac527, 0x68563c37 },
            { 0x2ffdf3214d9a4fa4, 0x9a83e79336350cee },
            { 0x622f3384, 0x6884feac },
            { 0xc11cc883, { 0x9e3cc608, 0x90eba4c0, 0xc3775cf8, 0x10eb4fee } },
            { 0x3ce3b80e, { 0x2c91b16326bf5f7f, 0xa21acf13c39485bc } },
        },

        { "abcdefghijklmnopqrstuvwxyz",
            { 0x7b3ea4bd, 0xd94ee9ea },
            { 0x68563c37, 0x0473b699 },
            { 0x9a83e79336350cee, 0x1f256c898952ae12 },
            { 0x6884feac, 0x99befd9e },
            { 0x9e3cc608, { 0x7de6cb3c, 0x62268725, 0x7c9f0b06, 0x5d7e2281 } },
            { 0x26bf5f7f, { 0xdbe75dee9ce0b3cb, 0x006c66d16f72b399, } },
        }
    };

    bool m1_32 = true;
    bool m2_32 = true;
    bool m2_64 = true;
    bool m3_32 = true;

    bool m3_128_x86 = true;
    bool m3_128_x64 = true;

    for (const auto &t: TESTS) {
        m1_32 = m1_32 && (t.m1_32.hash == util::hash::murmur1::hash_32 (t.key, strlen (t.key), t.m1_32.seed));
        m2_32 = m2_32 && (t.m2_32.hash == util::hash::murmur2::hash_32 (t.key, strlen (t.key), t.m2_32.seed));
        m2_64 = m2_64 && (t.m2_64.hash == util::hash::murmur2::hash_64 (t.key, strlen (t.key), t.m2_64.seed));
        m3_32 = m3_32 && (t.m3_32.hash == util::hash::murmur3::hash_32 (t.key, strlen (t.key), t.m3_32.seed));

        {
            auto result = util::hash::murmur3::hash_128_x86 (t.key, strlen (t.key), t.m3_128_x86.seed);
            bool success = t.m3_128_x86.hash == result;
            m3_128_x86 = m3_128_x86 && success;
        }

        {
            auto result = util::hash::murmur3::hash_128_x64 (t.key, strlen (t.key), t.m3_128_x64.seed);
            bool success = t.m3_128_x64.hash == result;
            m3_128_x64 = m3_128_x64 && success;
        }
    }

    tap.expect (m1_32, "murmur1_32");
    tap.expect (m2_32, "murmur2_32");
    tap.expect (m2_64, "murmur2_64");
    tap.expect (m3_32, "murmur3_32");
    tap.expect (m3_128_x86, "murmur3_128_x86");
    tap.expect (m3_128_x64, "murmur3_128_x64");
}


int
main (void)
{
    util::TAP::logger tap;

    test (tap);

    return tap.status ();
}
