#include "fixed.hpp"

#include "debug.hpp"

template <typename T, unsigned I, unsigned E>
void
test_simple (void)
{
    using fixed_t = util::fixed<T,I,E>;
    using integer_t = typename fixed_t::integer_t;

    const fixed_t lo = integer_t{0};
    const fixed_t hi = integer_t{1};

    CHECK_EQ (lo, lo);
    CHECK_EQ (hi, hi);

    CHECK_NEQ (hi, lo);
    CHECK_NEQ (lo, hi);

    CHECK_LT (lo, hi);
    CHECK_LE (lo, hi);
    CHECK_LE (lo, lo);

    CHECK_GT (hi, lo);
    CHECK_GE (lo, lo);
    CHECK_GE (hi, lo);
}


int
main (void)
{
    test_simple<signed,16,16> ();
    test_simple<signed,26, 6> ();
    test_simple<signed,32,32> ();

    test_simple<unsigned,16,16> ();
    test_simple<unsigned,26, 6> ();
    test_simple<unsigned,32,32> ();
}
