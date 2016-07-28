#include "roots/bisection.hpp"

#include "tap.hpp"

#include "maths.hpp"

using util::pow;


constexpr float order2 (float x) { return x * x + 3 * x - 7.f; }
constexpr float order4 (float x) { return   10 * pow (x, 4u)
                                          -270 * pow (x, 2u)
                                          -140 * pow (x, 1u)
                                          +1200; }

struct {
    float (*func) (float);
    float lo;
    float hi;
    float root;
    const char *msg;
} TESTS[] = {
    { order2, 0.f, 3.f, 0.5f * (std::sqrt (37.f) - 3.f), "order-2 bisection" },
    { order4, 0.f, 5.f, 2.f, "order-4 bisection" }
};


int
main (void)
{
    util::TAP::logger tap;

    for (const auto &t: TESTS) {
        constexpr float TOLERANCE = 0.00001f;
        auto root = util::roots::bisection (t.lo, t.hi, t.func, TOLERANCE);
        tap.expect_eq (root, t.root, "%s", t.msg);
    }

    return tap.status ();  
}
