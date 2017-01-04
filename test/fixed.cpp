#include "fixed.hpp"
#include "types/string.hpp"

#include "tap.hpp"


///////////////////////////////////////////////////////////////////////////////
template <typename T, unsigned I, unsigned E>
void
test_simple (util::TAP::logger &tap)
{
    using fixed   = util::fixed<T,I,E>;
    using integer = typename fixed::integer_type;

    const auto lo = fixed::from_integer (integer {0});
    const auto hi = fixed::from_integer (integer {1});

    std::ostringstream os;
    os << "fixed<" << type_to_string<T> () << ',' << I << ',' << E << '>';

    tap.expect_eq (lo, lo, "%s self equality", os.str ());
    tap.expect_eq (hi, hi, "%s self equality", os.str ());

    tap.expect_neq (hi, lo, "%s inequality", os.str ());
    tap.expect_neq (lo, hi, "%s inequality", os.str ());

    tap.expect_lt (lo, hi, "%s less than", os.str ());
    tap.expect_le (lo, hi, "%s less than equal", os.str ());
    tap.expect_le (lo, lo, "%s less than equal", os.str ());

    tap.expect_gt (hi, lo, "%s greater than", os.str ());
    tap.expect_ge (lo, lo, "%s greater than equal", os.str ());
    tap.expect_ge (hi, lo, "%s greater than equal", os.str ());
}


///////////////////////////////////////////////////////////////////////////////
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
