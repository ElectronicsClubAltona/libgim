#include "debug.hpp"
#include "range.hpp"
#include "tap.hpp"

#include <cstdlib>
#include <limits>

using namespace std;
using namespace util;

int
main (int, char **) {
    // Check some simple cases close to the edges of a unit range. Tests float rounding.
    CHECK ( range<double>::UNIT.contains ( 0.0));
    CHECK ( range<double>::UNIT.contains ( 0.5));
    CHECK ( range<double>::UNIT.contains ( 1.0));
    CHECK ( range<double>::UNIT.contains (std::numeric_limits<double>::min ()));
    CHECK (!range<double>::UNIT.contains (-0.00001));
    CHECK (!range<double>::UNIT.contains ( 1.00001));

    // Check edge cases of unit with integer values
    CHECK ( range<uint16_t>::UNIT.contains (0));
    CHECK ( range<uint16_t>::UNIT.contains (1));
    CHECK (!range<uint16_t>::UNIT.contains (2));
    CHECK (!range<uint16_t>::UNIT.contains (numeric_limits <uint16_t>::max ()));

    // Check the inclusivity of UNLIMITED with special floating values
    CHECK ( range<double>::UNLIMITED.contains (0.0));
    CHECK ( range<double>::UNLIMITED.contains (+numeric_limits<double>::infinity  ()));
    CHECK ( range<double>::UNLIMITED.contains (-numeric_limits<double>::infinity  ()));
    CHECK (!range<double>::UNLIMITED.contains ( numeric_limits<double>::quiet_NaN ()));

    // Check the inclusivity of UNLIMITED with some large numbers
    CHECK ( range<uint16_t>::UNLIMITED.contains (numeric_limits<uint16_t>::min()));
    CHECK ( range<uint16_t>::UNLIMITED.contains (numeric_limits<uint16_t>::max()));

    // Check inclusivity of MAX
    CHECK (!range<double>::MAX.contains ( numeric_limits<double>::infinity ()));
    CHECK (!range<double>::MAX.contains (-numeric_limits<double>::infinity ()));

    CHECK ( range<uint16_t>::MAX.contains (numeric_limits<uint16_t>::min()));
    CHECK ( range<uint16_t>::MAX.contains (numeric_limits<uint16_t>::max()));

    // Check that expansion via NaN is a noop
    {
        range<double> initial_nan (numeric_limits<double>::quiet_NaN (),
                                   numeric_limits<double>::quiet_NaN ());
        initial_nan.expand (1.0);
        CHECK_EQ (initial_nan.min, 1.0);
        CHECK_EQ (initial_nan.max, 1.0);
    }

    util::TAP::logger tap;
    tap.skip ("convert to TAP");
}

