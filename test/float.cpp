#include "float.hpp"

#include "debug.hpp"
#include "tap.hpp"
#include "types.hpp"

#include <limits>


///////////////////////////////////////////////////////////////////////////////
void
test_double (util::TAP::logger &tap)
{
    struct sized_test {
        util::ieee_double::uint_t bits;
        double floating;
    };

    sized_test tests[] = {
        { 0x3ff0000000000000,  1.0 },
        { 0x3ff0000000000001,  1.0 + std::numeric_limits<double>::epsilon () },
        { 0x3ff0000000000002,  1.0 + std::numeric_limits<double>::epsilon () * 2},
        { 0x4000000000000000,  2.0 },
        { 0xc000000000000000, -2.0 },
        { 0x0000000000000001,  std::numeric_limits<double>::denorm_min () },
        { 0x0010000000000000,  std::numeric_limits<double>::min () }, // min positive normal
        { 0x7fefffffffffffff,  std::numeric_limits<double>::max () }, // max
        { 0x0000000000000000,  0.0 },
        { 0x8000000000000000, -0.0 },
        { 0x7ff0000000000000,  std::numeric_limits<double>::infinity() },
        { 0xfff0000000000000, -std::numeric_limits<double>::infinity() },
        { 0x3fd5555555555555,  1.0 / 3.0 }
    };

    bool success = true;

    for (unsigned int i = 0; i < elems (tests); ++i) {
        util::ieee_double val;
        val.set_bits (tests[i].bits);

        success = success && util::exactly_equal (val, tests[i].floating);
    }

    tap.expect (success, "double precision bitwise equality");
}


///////////////////////////////////////////////////////////////////////////////
void
test_single (util::TAP::logger &tap)
{
    struct sized_test {
        util::ieee_single::uint_t bits;
        float floating;
    };

    sized_test tests[] = {
        { 0x3f800000,  1.0f },
        { 0xc0000000, -2.0f },
        { 0x7f7fffff, std::numeric_limits<float>::max () },

        { 0x00000000,   0.0f },
        { 0x80000000,  -0.0f },

        { 0x7f800000,  std::numeric_limits<float>::infinity () },
        { 0xff800000, -std::numeric_limits<float>::infinity () },

        { 0x3eaaaaab,  1.0f / 3.0f }
    };

    bool success = true;

    for (unsigned int i = 0; i < elems (tests); ++i) {
        util::ieee_single val;
        val.set_bits (tests[i].bits);

        success = success && util::exactly_equal (val, tests[i].floating);
    }

    tap.expect (success, "single precision bitwise equality");
}


///////////////////////////////////////////////////////////////////////////////
int
main (int, char **) {
    util::TAP::logger tap;

    test_single (tap);
    test_double (tap);

    return tap.status ();
}
