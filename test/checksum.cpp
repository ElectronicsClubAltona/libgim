
#include "hash/adler.hpp"
#include "hash/bsdsum.hpp"
#include "types.hpp"
#include "debug.hpp"

#include <cstdlib>
#include <cstring>

using namespace std;

static const char *ALPHABET = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

static const struct {
    uint32_t adler;
    uint16_t bsd;

    const char *data;
    size_t      size;
} TESTS[] = {
    { 0xDF5B150C, 0x52FB, ALPHABET, strlen (ALPHABET) },
    { 0x11E60398, 0x3DC8, "Wikipedia", strlen ("Wikipedia") }
};


int
main (int, char**) {
    for (unsigned i = 0; i < elems (TESTS); ++i) {
        CHECK_EQ (TESTS[i].adler, adler32 (TESTS[i].data, TESTS[i].size));
        CHECK_EQ (TESTS[i].bsd,   bsdsum  (TESTS[i].data, TESTS[i].size));
    }

    return EXIT_SUCCESS;
}
