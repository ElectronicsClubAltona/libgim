#include "tap.hpp"
#include "hash/crc.hpp"
#include "hash/simple.hpp"

#include <cstdint>
#include <utility>
#include <vector>


///////////////////////////////////////////////////////////////////////////////
static const
struct {
    uint32_t result;
    const char *dat;
    const char *msg;
} TESTS[] = {
    { 0x00000000, "",  "empty" },
    { 0xe8b7be43, "a", "single letter" },
    { 0xa684c7c6, "0123456789", "10 digits" },
    { 0x414fa339, "The quick brown fox jumps over the lazy dog", "quick brown fox" },
};


///////////////////////////////////////////////////////////////////////////////
int
main (int, char**)
{
    util::TAP::logger tap;

    for (const auto &t: TESTS) {
        auto first = t.dat;
        auto last  = first + strlen (t.dat);

        auto result = util::hash::simple<util::hash::crc32> (first, last);

        tap.expect_eq (t.result, result, "%s %xu", t.msg, result);
    }

    return tap.status ();
}