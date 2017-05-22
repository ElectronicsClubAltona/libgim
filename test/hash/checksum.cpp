
#include "hash/adler.hpp"
#include "hash/bsdsum.hpp"
#include "hash/simple.hpp"

#include "types.hpp"
#include "tap.hpp"
#include "debug.hpp"

#include <cstdlib>
#include <cstring>


///////////////////////////////////////////////////////////////////////////////
static const char *ALPHABET = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";


//-----------------------------------------------------------------------------
static const struct {
    uint32_t adler;
    uint16_t bsd;

    std::string msg;
    const char *data;
} TESTS[] = {
    { 0x00000001, 0x0000, "empty",     "" },
    { 0xDF5B150C, 0x52FB, "alphabet",  ALPHABET },
    { 0x11E60398, 0x3DC8, "wikipedia", "Wikipedia" },
    { 0x97B61069, 0x5555, "digits",    "12345678901234567890123456789012345678901234567890123456789012345678901234567890" },
};


///////////////////////////////////////////////////////////////////////////////
int
main (int, char**) {
    util::TAP::logger tap;

    for (const auto &t: TESTS) {
        tap.expect_eq (
            t.adler,
            util::hash::simple<util::hash::adler32> (
                (const void*)t.data,
                (const void*)(t.data + strlen (t.data))
            ),
            "adler checksum: %s", t.msg
        );

        tap.expect_eq (
            t.bsd,
            util::hash::simple<util::hash::bsdsum> (t.data, t.data + strlen (t.data)),
            "bsdsum checksum: %s", t.msg);
    }

    return tap.status ();
}
