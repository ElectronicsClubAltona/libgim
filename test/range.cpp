#include <cstdlib>
#include <limits>

#include "../debug.hpp"
#include "../range.hpp"

using namespace std;

int
main (int, char **) {
    check_hard ( range<double>::UNIT.includes ( 0.0));
    check_hard ( range<double>::UNIT.includes ( 0.5));
    check_hard ( range<double>::UNIT.includes ( 1.0));
    check_hard (!range<double>::UNIT.includes (-0.00001));
    check_hard (!range<double>::UNIT.includes ( 1.00001));

    check_hard ( range<uint16_t>::UNIT.includes (0));
    check_hard ( range<uint16_t>::UNIT.includes (1));
    check_hard (!range<uint16_t>::UNIT.includes (2));
    check_hard (!range<uint16_t>::UNIT.includes (numeric_limits <uint16_t>::max ()));

    check_hard ( range<double>::UNLIMITED.includes (0.0));
    check_hard ( range<double>::UNLIMITED.includes (+numeric_limits<double>::infinity  ()));
    check_hard ( range<double>::UNLIMITED.includes (-numeric_limits<double>::infinity  ()));
    check_hard (!range<double>::UNLIMITED.includes ( numeric_limits<double>::quiet_NaN ()));

    check_hard ( range<uint16_t>::UNLIMITED.includes (numeric_limits<uint16_t>::min()));
    check_hard ( range<uint16_t>::UNLIMITED.includes (numeric_limits<uint16_t>::max()));
    return EXIT_SUCCESS;
}

