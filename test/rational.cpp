#include "rational.hpp"
#include "tap.hpp"

using util::rational;


int
main (void)
{
    util::TAP::logger tap;

    {
        rational<int> val { -3, -2 };
        tap.expect (val.n == 3 && val.d == 2, "reduce double negatives at construction");
    }

    {
        rational<int> val { 5 };
        tap.expect (val.n == 5 && val.d == 1, "scalar construction");
    }

    {
        rational<int> a { 2, 3 };
        rational<int> b { 3, 2 };

        tap.expect_eq (a.inverse (), b, "inversion and equality");
    }

    {
        rational<int> val { 8, 12 };
        tap.expect_eq (val.reduced (), rational<int> { 2, 3 }, "factorisation");
    }

    return tap.status ();
}
