#include "polynomial.hpp"

#include "float.hpp"
#include "debug.hpp"
#include "tap.hpp"
#include "types.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

int
main (int, char**)
{
    auto nan = std::numeric_limits<float>::quiet_NaN ();

    struct {
        const char *name;
        std::array<float,4> coeffs;
        std::array<float,3> solutions;
    } CUBICS [] = {
        { "linear",         {  0,  0, -8,  7 }, { 7.f / 8, nan, nan } },
        { "quadratic",      {  0,  4, -8,  2 }, { 0.2928932188134524f, 1.7071067811865475f, nan } },
        { "unit cubic",     {  1,  0,  0,  0 }, { 0, nan, nan } },
        { "simple cubic",   {  1,  0,  2,  0 }, { 0, nan, nan } },
        { "twos cubic",     { -2,  2, -2,  2 }, { 1, nan, nan } },
        { "a & d cubic",    {  1,  0,  0,  2 }, { -std::cbrt(2.f), nan, nan } },
        { "cubic 1 real 1", {  2, -3,  8, -2 }, { 0.2728376017309678f, nan, nan } },
        { "cubic 1 real 2", {  1,  4, -8,  7 }, { -5.638871145892407f, nan, nan } },
        { "cubic all real", {  1,  3, -6, -8 }, { -4, -1, 2, } },
        { "cubic 1 uniq",   {  1, -3,  3, -1 }, { 1, nan, nan } },
    };

    util::TAP::logger test;

    for (auto &i: CUBICS) {
        std::array<float,3> s = util::polynomial::roots<3> (i.coeffs);

        std::sort (s.begin (), s.end ());

        bool ok = true;

        for (size_t j = 0; j < 3; ++j) {
            if (std::isnan (i.solutions[j])) {
                if (!std::isnan (s[j]))
                    ok = false;

                continue;
            }

            if (!almost_equal (i.solutions[j], s[j]))
                ok = false;
        }

        test.expect (ok, i.name);
    }

    return 0;
}
