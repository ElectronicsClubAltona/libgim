#include "hash/md5.hpp"

#include "tap.hpp"

#include <iostream>
#include <cstring>

using util::hash::MD5;


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
main (int, char**) {
    static const struct {
        std::vector<uint8_t> input;
        MD5::digest_t  output;
        const char    *msg;
    } TESTS[] = {
        { {},
          { { 0xd4, 0x1d, 0x8c, 0xd9, 0x8f, 0x00, 0xb2, 0x04,
              0xe9, 0x80, 0x09, 0x98, 0xec, 0xf8, 0x42, 0x7e } },
          "empty"
        },
        { "a"_u8s,
          { { 0x0c, 0xc1, 0x75, 0xb9, 0xc0, 0xf1, 0xb6, 0xa8,
              0x31, 0xc3, 0x99, 0xe2, 0x69, 0x77, 0x26, 0x61 } },
          "1 byte"
        },
        { "abc"_u8s,
          { { 0x90, 0x01, 0x50, 0x98, 0x3c, 0xd2, 0x4f, 0xb0,
              0xd6, 0x96, 0x3f, 0x7d, 0x28, 0xe1, 0x7f, 0x72 } },
          "3 bytes"
        },
        { "message digest"_u8s,
          { { 0xf9, 0x6b, 0x69, 0x7d, 0x7c, 0xb7, 0x93, 0x8d,
              0x52, 0x5a, 0x2f, 0x31, 0xaa, 0xf1, 0x61, 0xd0 } },
          "14 bytes, text"
        },
        { "abcdefghijklmnopqrstuvwxyz"_u8s,
          { { 0xc3, 0xfc, 0xd3, 0xd7, 0x61, 0x92, 0xe4, 0x00,
              0x7d, 0xfb, 0x49, 0x6c, 0xca, 0x67, 0xe1, 0x3b } },
          "14 bytes, alphabet"
        },
        { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"_u8s,
          { { 0xd1, 0x74, 0xab, 0x98, 0xd2, 0x77, 0xd9, 0xf5,
              0xa5, 0x61, 0x1c, 0x2c, 0x9f, 0x41, 0x9d, 0x9f } },
          "62 bytes"

        },
        { "12345678901234567890123456789012345678901234567890123456789012345678901234567890"_u8s,
          { { 0x57, 0xed, 0xf4, 0xa2, 0x2b, 0xe3, 0xc9, 0x55,
              0xac, 0x49, 0xda, 0x2e, 0x21, 0x07, 0xb6, 0x7a } },
          "80 bytes"
        }
    };

    util::TAP::logger tap;

    MD5 h;
    for (const auto &t: TESTS)
        tap.expect_eq (h (t.input), t.output, "%s", t.msg);


    // check that appending or prepending an empty data view doesn't change
    // the hash for a couple of data lengths
    for (auto l: { 0, 1, 8, 64, 80}) {
        std::vector<uint8_t> data (l);
        std::iota (std::begin (data), std::end (data), 0);
        tap.expect_eq (h (nullptr, data), h (data), "empty-full vs full hash equality, %! bytes", l);
        tap.expect_eq (h (data, nullptr), h (data), "full-empty vs full hash equality, %! bytes", l);
    }

    for (auto l: { 2, 64, 80}) {
        std::vector<uint8_t> data (l);
        std::iota (std::begin (data), std::end (data), 0);
        util::view root {data};
        auto [a,b] = root.split (root.begin () + l / 2);
        tap.expect_eq (h (a,b), h(root), "split data hash equality, %! bytes", l);
    };

    return tap.status ();
}
