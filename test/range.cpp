#include <cstdlib>
#include <limits>

#include "../debug.hpp"
#include "../range.hpp"

using namespace std;
using namespace util;

int
main (int, char **) {
    CHECK_HARD ( range<double>::UNIT.contains ( 0.0));
    CHECK_HARD ( range<double>::UNIT.contains ( 0.5));
    CHECK_HARD ( range<double>::UNIT.contains ( 1.0));
    CHECK_HARD (!range<double>::UNIT.contains (-0.00001));
    CHECK_HARD (!range<double>::UNIT.contains ( 1.00001));

    CHECK_HARD ( range<uint16_t>::UNIT.contains (0));
    CHECK_HARD ( range<uint16_t>::UNIT.contains (1));
    CHECK_HARD (!range<uint16_t>::UNIT.contains (2));
    CHECK_HARD (!range<uint16_t>::UNIT.contains (numeric_limits <uint16_t>::max ()));

    CHECK_HARD ( range<double>::UNLIMITED.contains (0.0));
    CHECK_HARD ( range<double>::UNLIMITED.contains (+numeric_limits<double>::infinity  ()));
    CHECK_HARD ( range<double>::UNLIMITED.contains (-numeric_limits<double>::infinity  ()));
    CHECK_HARD (!range<double>::UNLIMITED.contains ( numeric_limits<double>::quiet_NaN ()));

    CHECK_HARD ( range<uint16_t>::UNLIMITED.contains (numeric_limits<uint16_t>::min()));
    CHECK_HARD ( range<uint16_t>::UNLIMITED.contains (numeric_limits<uint16_t>::max()));
    return EXIT_SUCCESS;
}

