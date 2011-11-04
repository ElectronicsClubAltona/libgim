
#include "adler.hpp"
#include "bsdsum.hpp"
#include "types.hpp"

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
    { 0xDF5B150C, 0x52FB, ALPHABET, strlen (ALPHABET) }
};


int
main (int, char**) {
    for (unsigned i = 0; i < elems (TESTS); ++i) {
        check_eq (TESTS[i].adler, adler32 (TESTS[i].data, TESTS[i].size));
        check_eq (TESTS[i].bsd,   bsdsum (TESTS[i].data, TESTS[i].size));
    }

    return EXIT_SUCCESS;
}
