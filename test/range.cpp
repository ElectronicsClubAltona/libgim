#include "range.hpp"

#include "tap.hpp"

#include <cstdlib>
#include <limits>

int
main (int, char **)
{
    util::TAP::logger tap;

    // Check some simple cases close to the edges of a unit range. Tests float rounding.
    tap.expect ( util::range<double>::UNIT.contains ( 0.0), "floating unit contains 0");
    tap.expect ( util::range<double>::UNIT.contains ( 1.0), "floating unit contains 1");
    tap.expect ( util::range<double>::UNIT.contains (std::numeric_limits<double>::min ()), "floating unit contains min");
    tap.expect (!util::range<double>::UNIT.contains (-0.00001), "doesn't contain fractionally low value");
    tap.expect (!util::range<double>::UNIT.contains ( 1.00001), "doesn't contain fractionally high value");

    // Check edge cases of unit with integer values
    tap.expect ( util::range<uint16_t>::UNIT.contains (0), "unsigned unit contains 0");
    tap.expect ( util::range<uint16_t>::UNIT.contains (1), "unsigned unit contains 1");
    tap.expect (!util::range<uint16_t>::UNIT.contains (2), "unsigned unit doesn't contain 2");
    tap.expect (!util::range<uint16_t>::UNIT.contains (std::numeric_limits <uint16_t>::max ()), "unsigned unit doesn't contain max");

    // Check the inclusivity of UNLIMITED with special floating values
    tap.expect ( util::range<double>::UNLIMITED.contains (0.0), "floating unlimited contains 0");
    tap.expect ( util::range<double>::UNLIMITED.contains (+std::numeric_limits<double>::infinity  ()), "floating unlimited contains +INF");
    tap.expect ( util::range<double>::UNLIMITED.contains (-std::numeric_limits<double>::infinity  ()), "floating unlimited contains -INF");
    tap.expect (!util::range<double>::UNLIMITED.contains ( std::numeric_limits<double>::quiet_NaN ()), "floating unlimited doesn't contain NAN");

    // Check the inclusivity of UNLIMITED with some large numbers
    tap.expect ( util::range<uint16_t>::UNLIMITED.contains (std::numeric_limits<uint16_t>::min()), "floating unlimited contains min");
    tap.expect ( util::range<uint16_t>::UNLIMITED.contains (std::numeric_limits<uint16_t>::max()), "floating unlimited contains max");

    // Check inclusivity of MAX
    tap.expect (!util::range<double>::MAX.contains ( std::numeric_limits<double>::infinity ()), "floating max contains +INF");
    tap.expect (!util::range<double>::MAX.contains (-std::numeric_limits<double>::infinity ()), "floating max contains -INF");

    tap.expect ( util::range<uint16_t>::MAX.contains (std::numeric_limits<uint16_t>::min()), "unsigned max contains min");
    tap.expect ( util::range<uint16_t>::MAX.contains (std::numeric_limits<uint16_t>::max()), "unsigned max contains max");

    // Check that expansion via NaN is a noop
    {
        util::range<double> initial_nan (std::numeric_limits<double>::quiet_NaN (),
                                         std::numeric_limits<double>::quiet_NaN ());
        initial_nan.expand (1.0);

        tap.expect_eq (initial_nan.min, 1.0, "NAN expansion noop for min");
        tap.expect_eq (initial_nan.max, 1.0, "NAN expansion noop for max");
    }

    return tap.status ();
}
