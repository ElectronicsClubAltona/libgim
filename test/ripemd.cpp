#include "../debug.hpp"
#include "../types.hpp"
#include "../hash/ripemd.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>

using util::hash::RIPEMD;

static const
struct {
    const char       *input;
    RIPEMD::digest_t  output;
} TESTS[] = {
    {
        "",
        { 0x9c, 0x11, 0x85, 0xa5, 0xc5, 0xe9, 0xfc, 0x54, 0x61, 0x28,
          0x08, 0x97, 0x7e, 0xe8, 0xf5, 0x48, 0xb2, 0x25, 0x8d, 0x31 },
        // 128: cdf26213a150dc3ecb610f18f6b38b46
        // 160: 9c1185a5c5e9fc54612808977ee8f548b2258d31
        // 256: 02ba4c4e5f8ecd1877fc52d64d30e37a2d9774fb1e5d026380ae0168e3c5522d
        // 320: 22d65d5661536cdc75c1fdf5c6de7b41b9f27325ebc61e8557177d705a0ec880151c3a32a00899b8
    },

    {
        "a",
        { 0x0b, 0xdc, 0x9d, 0x2d, 0x25, 0x6b, 0x3e, 0xe9, 0xda, 0xae,
          0x34, 0x7b, 0xe6, 0xf4, 0xdc, 0x83, 0x5a, 0x46, 0x7f, 0xfe },
        // 128: 86be7afa339d0fc7cfc785e72f578d33
        // 160: 0bdc9d2d256b3ee9daae347be6f4dc835a467ffe
        // 256: f9333e45d857f5d90a91bab70a1eba0cfb1be4b0783c9acfcd883a9134692925
        // 320: ce78850638f92658a5a585097579926dda667a5716562cfcf6fbe77f63542f99b04705d6970dff5d
    },

    {
        "abc",
        { 0x8e, 0xb2, 0x08, 0xf7, 0xe0, 0x5d, 0x98, 0x7a, 0x9b, 0x04,
          0x4a, 0x8e, 0x98, 0xc6, 0xb0, 0x87, 0xf1, 0x5a, 0x0b, 0xfc },
        // 128: c14a12199c66e4ba84636b0f69144c77
        // 160: 8eb208f7e05d987a9b044a8e98c6b087f15a0bfc
        // 256: afbd6e228b9d8cbbcef5ca2d03e6dba10ac0bc7dcbe4680e1e42d2e975459b65
        // 320: de4c01b3054f8930a79d09ae738e92301e5a17085beffdc1b8d116713e74f82fa942d64cdbc4682d
    },

    {
        "message digest",
        { 0x5d, 0x06, 0x89, 0xef, 0x49, 0xd2, 0xfa, 0xe5, 0x72, 0xb8,
          0x81, 0xb1, 0x23, 0xa8, 0x5f, 0xfa, 0x21, 0x59, 0x5f, 0x36 },
        // 128: 9e327b3d6e523062afc1132d7df9d1b8
        // 160: 5d0689ef49d2fae572b881b123a85ffa21595f36
        // 256: 87e971759a1ce47a514d5c914c392c9018c7c46bc14465554afcdf54a5070c0e
        // 320: 3a8e28502ed45d422f68844f9dd316e7b98533fa3f2a91d29f84d425c88d6b4eff727df66a7c0197
    },

    {
        "abcdefghijklmnopqrstuvwxyz",
        { 0xf7, 0x1c, 0x27, 0x10, 0x9c, 0x69, 0x2c, 0x1b, 0x56, 0xbb,
          0xdc, 0xeb, 0x5b, 0x9d, 0x28, 0x65, 0xb3, 0x70, 0x8d, 0xbc },
        // 128: fd2aa607f71dc8f510714922b371834e
        // 160: f71c27109c692c1b56bbdceb5b9d2865b3708dbc
        // 256: 649d3034751ea216776bf9a18acc81bc7896118a5197968782dd1fd97d8d5133
        // 320: cabdb1810b92470a2093aa6bce05952c28348cf43ff60841975166bb40ed234004b8824463e6b009
    },

    {
        "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
        { 0x12, 0xa0, 0x53, 0x38, 0x4a, 0x9c, 0x0c, 0x88, 0xe4, 0x05,
          0xa0, 0x6c, 0x27, 0xdc, 0xf4, 0x9a, 0xda, 0x62, 0xeb, 0x2b },
        // 128: a1aa0689d0fafa2ddc22e88b49133a06
        // 160: 12a053384a9c0c88e405a06c27dcf49ada62eb2b
        // 256: 3843045583aac6c8c8d9128573e7a9809afb2a0f34ccc36ea9e72f16f6368e3f
        // 320: d034a7950cf722021ba4b84df769a5de2060e259df4c9bb4a4268c0e935bbc7470a969c9d072a1ac
    },

    {
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
        { 0xb0, 0xe2, 0x0b, 0x6e, 0x31, 0x16, 0x64, 0x02, 0x86, 0xed,
          0x3a, 0x87, 0xa5, 0x71, 0x30, 0x79, 0xb2, 0x1f, 0x51, 0x89 },
        // 128: d1e959eb179c911faea4624c60c5c702
        // 160: b0e20b6e3116640286ed3a87a5713079b21f5189
        // 256: 5740a408ac16b720b84424ae931cbb1fe363d1d0bf4017f1a89f7ea6de77a0b8
        // 320: ed544940c86d67f250d232c30b7b3e5770e0c60c8cb9a4cafe3b11388af9920e1b99230b843c86a4
    },

    {
        "12345678901234567890123456789012345678901234567890123456789012345678901234567890",
        { 0x9b, 0x75, 0x2e, 0x45, 0x57, 0x3d, 0x4b, 0x39, 0xf4, 0xdb,
          0xd3, 0x32, 0x3c, 0xab, 0x82, 0xbf, 0x63, 0x32, 0x6b, 0xfb }
        // 128: 3f45ef194732c2dbb2c4a2c769795fa3
        // 160: 9b752e45573d4b39f4dbd3323cab82bf63326bfb
        // 256: 06fdcc7a409548aaf91368c06a6275b553e3f099bf0ea4edfd6778df89a890dd
        // 320: 557888af5f6d8ed62ab66945c6d2a0a47ecd5341e915eb8fea1d0524955f825dc717e4a008ab2d42
    }
};

// 1 million times "a"
// 128: 4a7f5723f954eba1216c9d8f6320431f
// 160: 52783243c1697bdbe16d37f97f68f08325dc1528
// 256: ac953744e10e31514c150d4d8d7b677342e33399788296e43ae4850ce4f97978
// 320: bdee37f4371e20646b8b0d862dda16292ae36f40965e8c8509e63d1dbddecc503e2b63eb9245bb66

int
main(int, char**) {
    // Check against simple test vectors
    for (size_t i = 0; i < elems (TESTS); ++i) {
        std::cout << "testing '" << TESTS[i].input << "'\n";

        util::hash::RIPEMD obj;
        obj.update (reinterpret_cast<const uint8_t*> (TESTS[i].input),
                    strlen (TESTS[i].input));
        obj.finish ();

        CHECK (obj.digest () == TESTS[i].output);
    }

    // Perform 'million-a' check
    static const size_t CHUNK_WIDTH = 1000;
    util::hash::RIPEMD obj;

    for (size_t i = 0; i < 1000000; i += CHUNK_WIDTH) {
        uint8_t data[CHUNK_WIDTH];
        memset (data, 'a', sizeof (data));

        obj.update (data, sizeof (data));
    }

    obj.finish ();
    static const util::hash::RIPEMD::digest_t MILLION {
        0x52, 0x78, 0x32, 0x43, 0xc1, 0x69, 0x7b, 0xdb, 0xe1, 0x6d,
        0x37, 0xf9, 0x7f, 0x68, 0xf0, 0x83, 0x25, 0xdc, 0x15, 0x28
    };
    CHECK (obj.digest () == MILLION);

    return EXIT_SUCCESS;
}
