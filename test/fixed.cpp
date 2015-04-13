#include "fixed.hpp"

#include "tap.hpp"

template <typename T, unsigned I, unsigned E>
void
test_simple (util::TAP::logger &tap)
{
    using fixed_t = util::fixed<T,I,E>;
    using integer_t = typename fixed_t::integer_t;

    const fixed_t lo = integer_t{0};
    const fixed_t hi = integer_t{1};

    std::ostringstream os;
    os << "fixed<" << type_to_string<T> () << ',' << I << ',' << E << '>';

    tap.expect_eq (lo, lo, os.str () + " self equality");
    tap.expect_eq (hi, hi, os.str () + " self equality");

    tap.expect_neq (hi, lo, os.str () + " inequality");
    tap.expect_neq (lo, hi, os.str () + " inequality");

    tap.expect_lt (lo, hi, os.str () + " less than");
    tap.expect_le (lo, hi, os.str () + " less than equal");
    tap.expect_le (lo, lo, os.str () + " less than equal");

    tap.expect_gt (hi, lo, os.str () + " greater than");
    tap.expect_ge (lo, lo, os.str () + " greater than equal");
    tap.expect_ge (hi, lo, os.str () + " greater than equal");
}


int
main (void)
{
    util::TAP::logger tap;

    test_simple<signed,16,16> (tap);
    test_simple<signed,26, 6> (tap);
    test_simple<signed,32,32> (tap);

    test_simple<unsigned,16,16> (tap);
    test_simple<unsigned,26, 6> (tap);
    test_simple<unsigned,32,32> (tap);
}
