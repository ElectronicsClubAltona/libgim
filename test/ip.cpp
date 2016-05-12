
#include "ip.hpp"

#include "debug.hpp"
#include "platform.hpp"
#include "tap.hpp"
#include "types.hpp"

#include <cstdlib>


//-----------------------------------------------------------------------------
static void
test_good (util::TAP::logger &tap)
{
    static const struct {
        const char  *str;
        ipv4::ip ip;
        const char *msg;
    } TESTS[] = {
        {         "0.0.0.0", {   0,   0,   0,   0 }, "null" },
        { "255.255.255.255", { 255, 255, 255, 255 }, "full" },
        {       "127.0.0.1", { 127,   0,   0,   1 }, "localhost" }
    };

    for (const auto &i: TESTS) {
        tap.expect_eq (ipv4::ip::parse (i.str), i.ip, i.msg);
    }
}


//-----------------------------------------------------------------------------
static void
test_bad (util::TAP::logger &tap)
{
    static const struct {
        const char *str;
        const char *msg;
    } TESTS[] = {
        { "::1", "ipv6" },
        { "foo", "alpha" },
        { "", "empty" },
        { "256.0.0.1", "overflow" }
    };

    for (const auto &i: TESTS) {
        tap.expect_throw<ipv4::error> ([&] { ipv4::ip::parse (i.str); }, i.msg);
    }
}


//-----------------------------------------------------------------------------
int
main (int, char **) {
    util::TAP::logger tap;

    test_good (tap);
    test_bad (tap);

    return tap.status ();
}
