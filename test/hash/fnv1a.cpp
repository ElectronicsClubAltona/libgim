#include "hash/fnv1a.hpp"
#include "tap.hpp"


///////////////////////////////////////////////////////////////////////////////
static const struct {
    uint32_t h32;
    uint64_t h64;
    const char *data;
} TESTS[] = {
    { 0x811c9dc5, 0xcbf29ce484222325, "" },
    { 0xe40c292c, 0xaf63dc4c8601ec8c, "a" },
    { 0xbf9cf968, 0x85944171f73967e8, "foobar" },
};


///////////////////////////////////////////////////////////////////////////////
int
main (void)
{
    util::TAP::logger tap;

    const util::hash::fnv1a<uint32_t> h32;
    const util::hash::fnv1a<uint64_t> h64;

    for (const auto &t: TESTS) {
        tap.expect_eq (h32 (util::view{t.data}.cast <const uint8_t> ()), t.h32, "fnv1a32: '%s'", t.data);
        tap.expect_eq (h64 (util::view{t.data}.cast <const uint8_t> ()), t.h64, "fnv1a64: '%s'", t.data);
    }

    return tap.status ();
}