#include "tap.hpp"

#include "maths/fast.hpp"


constexpr float
threshold (float a, float b)
{
    constexpr float PARTS = 100;
    return std::abs (a + b) / 2 / PARTS;
}


int
main (void)
{
    util::TAP::logger tap;


    {
        auto a = util::maths::fast::log2 (3.456f);
        auto b = std::log2 (3.456f);
        tap.expect_lt (std::abs (a - b), threshold (a, b), "fast log2");
    }

    {
        auto a = util::maths::fast::pow2 (-100.f);
        auto b = std::pow (2.f, -100.f);
        tap.expect_lt (std::abs (a - b), threshold (a, b), "fast pow2");
    }

    {
        auto a = util::maths::fast::pow (0.8f, 100.f);
        auto b = std::pow (0.8f, 100.f);
        tap.expect_lt (std::abs (a - b), threshold (a, b), "fast pow");
    }

    return tap.status ();
}
