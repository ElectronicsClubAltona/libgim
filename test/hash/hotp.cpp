#include "hash/hotp.hpp"

#include "debug.hpp"
#include "tap.hpp"
#include "types.hpp"

using util::hash::HOTP;

int
main (int, char**)
{
    HOTP h ("12345678901234567890", 0);

    const unsigned EXPECTED[] = {
        755224,
        287082,
        359152,
        969429,
        338314,
        254676,
        287922,
        162583,
        399871,
        520489,
    };

    
    util::TAP::logger tap;

    for (auto &i: EXPECTED)
        tap.expect_eq (i, h.value (), "sequence");

    return tap.status ();
}
