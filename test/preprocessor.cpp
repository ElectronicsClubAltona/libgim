#include "tap.hpp"
#include "preprocessor.hpp"


#define PLUS(X,Y) ((X)+(Y))
#define SIX(...) 6

int
main (void)
{
    util::TAP::logger tap;

    int foo = 1, bar = 2, foobar = 3;
    tap.expect_eq (foo + bar, PASTE(foo,bar), "PASTE");
    tap.expect_eq (REDUCE1(PLUS, 1, 2, 3, 4), 10, "REDUCE0");
    tap.expect_eq (MAP0(SIX, 9), 6, "MAP0");
    tap.expect_eq (MAP1(PLUS, 6, 9), 15, "MAP1");
    tap.expect_eq (VA_ARGS_COUNT (1, 2, 3, 4, 5, , 7, 8, 9), 9, "VA_ARGS_COUNT");

    return tap.status ();
}
