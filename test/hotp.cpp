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

    for (size_t i = 0; i < elems (EXPECTED); ++i)
        CHECK_EQ (EXPECTED[i], h.value ());

    util::TAP::logger tap;
    tap.todo ("convert to TAP");
}
