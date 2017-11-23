#include "extent.hpp"
#include "tap.hpp"
#include "types.hpp"

int
main (void)
{
    util::TAP::logger tap;

    {
        constexpr const util::extent2u hi { 8, 4 };
        constexpr const util::extent2u lo { 6, 2 };
        tap.expect_eq (lo, hi.contracted (2), "extent scalar contraction by value");
    }

    {
        static constexpr util::point2u EXPECTED[] = {
            { 0, 0 }, { 1, 0 }, { 2, 0 },
            { 0, 1 }, { 1, 1 }, { 2, 1 },
            { 0, 2 }, { 1, 2 }, { 2, 2 },
        };

        size_t offset = 0;
        unsigned success = 0;

        for (auto p: util::extent_range2u ({3, 3}))
            success += EXPECTED[offset++] == p ? 1 : 0;

        tap.expect_eq (success, std::size (EXPECTED), "extent_range2u iteration");
    }

    return tap.status ();
}
