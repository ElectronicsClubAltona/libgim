#include "fixed.hpp"

#include "debug.hpp"

template <unsigned I, unsigned E>
void
test_simple (void)
{
    using fixed_t = util::fixed<I,E>;
    using uint_t  = typename fixed_t::uint_t;

    const fixed_t lo = uint_t{0};
    const fixed_t hi = uint_t{1};

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
    test_simple<16,16> ();
    test_simple<26, 6> ();
    test_simple<32,32> ();
}
