#include <cstdlib>
#include <limits>

#include "../debug.hpp"
#include "../range.hpp"

using namespace std;

int
main (int, char **) {
    check_hard ( range<double>::UNIT.contains ( 0.0));
    check_hard ( range<double>::UNIT.contains ( 0.5));
    check_hard ( range<double>::UNIT.contains ( 1.0));
    check_hard (!range<double>::UNIT.contains (-0.00001));
    check_hard (!range<double>::UNIT.contains ( 1.00001));

    check_hard ( range<uint16_t>::UNIT.contains (0));
    check_hard ( range<uint16_t>::UNIT.contains (1));
    check_hard (!range<uint16_t>::UNIT.contains (2));
    check_hard (!range<uint16_t>::UNIT.contains (numeric_limits <uint16_t>::max ()));

    check_hard ( range<double>::UNLIMITED.contains (0.0));
    check_hard ( range<double>::UNLIMITED.contains (+numeric_limits<double>::infinity  ()));
    check_hard ( range<double>::UNLIMITED.contains (-numeric_limits<double>::infinity  ()));
    check_hard (!range<double>::UNLIMITED.contains ( numeric_limits<double>::quiet_NaN ()));

    check_hard ( range<uint16_t>::UNLIMITED.contains (numeric_limits<uint16_t>::min()));
    check_hard ( range<uint16_t>::UNLIMITED.contains (numeric_limits<uint16_t>::max()));
    return EXIT_SUCCESS;
}

