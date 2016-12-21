#include "range.hpp"

#include "tap.hpp"

#include <cstdlib>
#include <limits>

int
main (int, char **)
{
    util::TAP::logger tap;

    // Check some simple cases close to the edges of a unit range. Tests float rounding.
    tap.expect ( util::range<double>::unit ().contains ( 0.0), "floating unit contains 0");
    tap.expect ( util::range<double>::unit ().contains ( 1.0), "floating unit contains 1");
    tap.expect ( util::range<double>::unit ().contains (std::numeric_limits<double>::min ()), "floating unit contains min");
    tap.expect (!util::range<double>::unit ().contains (-0.00001), "doesn't contain fractionally low value");
    tap.expect (!util::range<double>::unit ().contains ( 1.00001), "doesn't contain fractionally high value");

    // Check edge cases of unit with integer values
    tap.expect ( util::range<uint16_t>::unit ().contains (0), "unsigned unit contains 0");
    tap.expect ( util::range<uint16_t>::unit ().contains (1), "unsigned unit contains 1");
    tap.expect (!util::range<uint16_t>::unit ().contains (2), "unsigned unit doesn't contain 2");
    tap.expect (!util::range<uint16_t>::unit ().contains (std::numeric_limits <uint16_t>::max ()), "unsigned unit doesn't contain max");

    // Check the inclusivity of unlimited with special floating values
    tap.expect ( util::range<double>::unlimited ().contains (0.0), "floating unlimited contains 0");
    tap.expect ( util::range<double>::unlimited ().contains (+std::numeric_limits<double>::infinity  ()), "floating unlimited contains +INF");
    tap.expect ( util::range<double>::unlimited ().contains (-std::numeric_limits<double>::infinity  ()), "floating unlimited contains -INF");
    tap.expect (!util::range<double>::unlimited ().contains ( std::numeric_limits<double>::quiet_NaN ()), "floating unlimited doesn't contain NAN");

    // Check the inclusivity of unlimited with some large numbers
    tap.expect ( util::range<uint16_t>::unlimited ().contains (std::numeric_limits<uint16_t>::min()), "floating unlimited contains min");
    tap.expect ( util::range<uint16_t>::unlimited ().contains (std::numeric_limits<uint16_t>::max()), "floating unlimited contains max");

    // Check inclusivity of max
    tap.expect (!util::range<double>::max ().contains ( std::numeric_limits<double>::infinity ()), "floating max contains +INF");
    tap.expect (!util::range<double>::max ().contains (-std::numeric_limits<double>::infinity ()), "floating max contains -INF");

    tap.expect ( util::range<uint16_t>::max ().contains (std::numeric_limits<uint16_t>::min()), "unsigned max contains min");
    tap.expect ( util::range<uint16_t>::max ().contains (std::numeric_limits<uint16_t>::max()), "unsigned max contains max");

    // Check that expansion via NaN is a noop
    {
        util::range<double> initial_nan (std::numeric_limits<double>::quiet_NaN (),
                                         std::numeric_limits<double>::quiet_NaN ());
        initial_nan.expand (1.0);

        tap.expect_eq (initial_nan.lo, 1.0, "NAN expansion noop for lo");
        tap.expect_eq (initial_nan.hi, 1.0, "NAN expansion noop for hi");
    }

    return tap.status ();
}
