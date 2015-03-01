#include "hash/hmac.hpp"
#include "types.hpp"
#include "debug.hpp"

#include <cstring>
#include <iostream>
#include <vector>

using util::hash::HMAC;


//-----------------------------------------------------------------------------
std::vector<uint8_t>
to_vector (const char *str)
{
    return std::vector<uint8_t> (str, str + strlen (str));
}


//-----------------------------------------------------------------------------
static const struct {
    std::vector<uint8_t> key;
    std::vector<uint8_t> dat;
    HMAC::digest_t res;
} TESTS[] = {
    // RFC 2104 test data, MD5
    {
        { 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
          0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b  },
        { 'H', 'i', ' ', 'T', 'h', 'e', 'r', 'e' },
        { { 0x92, 0x94, 0x72, 0x7a, 0x36, 0x38, 0xbb, 0x1c,
            0x13, 0xf4, 0x8e, 0xf8, 0x15, 0x8b, 0xfc, 0x9d } }
    },

    {
        to_vector ("Jefe"),
        to_vector ("what do ya want for nothing?"),
        { { 0x75, 0x0c, 0x78, 0x3e, 0x6a, 0xb0, 0xb5, 0x03,
            0xea, 0xa8, 0x6e, 0x31, 0x0a, 0x5d, 0xb7, 0x38 } }
    },

    {
        { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
          0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA  },
        { 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD,
          0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD,
          0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD,
          0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD,
          0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD,
          0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD,
          0xDD, 0xDD },
        { { 0x56, 0xbe, 0x34, 0x52, 0x1d, 0x14, 0x4c, 0x88,
            0xdb, 0xb8, 0xc7, 0x33, 0xf0, 0xe8, 0xb3, 0xf6 } }
    },

    // RFC 2202 test data, MD5
    {
        { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
          0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
          0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
          0x19 },
        { 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD,
          0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD,
          0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD,
          0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD,
          0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD,
          0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD,
          0xCD, 0xCD },
        { { 0x69, 0x7e, 0xaf, 0x0a, 0xca, 0x3a, 0x3a, 0xea,
            0x3a, 0x75, 0x16, 0x47, 0x46, 0xff, 0xaa, 0x79 } }
    },

    {
        { 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
          0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c },
        to_vector ("Test With Truncation"),
        { { 0x56, 0x46, 0x1e, 0xf2, 0x34, 0x2e, 0xdc, 0x00,
            0xf9, 0xba, 0xb9, 0x95, 0x69, 0x0e, 0xfd, 0x4c } }
        // digest-96: 0x56461ef2342edc00f9bab995
    },

    {
        { 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
          0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
          0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
          0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
          0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
          0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
          0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
          0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
          0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
          0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa },
        to_vector ("Test Using Larger Than Block-Size Key - Hash Key First"),
        { { 0x6b, 0x1a, 0xb7, 0xfe, 0x4b, 0xd7, 0xbf, 0x8f,
            0x0b, 0x62, 0xe6, 0xce, 0x61, 0xb9, 0xd0, 0xcd } }
    },

    {
        { 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
          0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
          0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
          0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
          0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
          0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
          0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
          0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
          0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
          0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa },
        to_vector ("Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data"),
        { { 0x6f, 0x63, 0x0f, 0xad, 0x67, 0xcd, 0xa0, 0xee,
            0x1f, 0xb1, 0xf5, 0x62, 0xdb, 0x3a, 0xa5, 0x3e } }
    }
};


//-----------------------------------------------------------------------------
int
main (int, char**)
{
    unsigned i = 0;
    for (const auto &t: TESTS) {
        std::cerr << i++ << ": " << t.key.size () << ", " << t.dat.size () << '\n';

        util::hash::HMAC h (t.key.data (), t.key.size ());
        h.update (t.dat.data (), t.dat.size ());
        h.finish ();

        if (h.digest () != t.res)
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
