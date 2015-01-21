#include "polynomial.hpp"

#include "float.hpp"
#include "debug.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

int
main (int, char**)
{
    auto nan = std::numeric_limits<float>::quiet_NaN ();

    struct {
        std::array<float,4> coeffs;
        std::array<float,3> solutions;
    } CUBICS [] = {
        { {  0,  0, -8,  7 }, { 7.f / 8, nan, nan } },
        { {  0,  4, -8,  2 }, { 0.292893f, 1.70710f, nan } },
        { {  1,  0,  0,  0 }, { 0, nan, nan } },
        { {  1,  0,  2,  0 }, { 0, nan, nan } },
        { { -2,  2, -2,  2 }, { 1, nan, nan } },
        { {  1,  0,  0,  2 }, { -std::cbrt(2.f), nan, nan } },
        { {  2, -3,  8, -2 }, { 0.2728376017309678f, nan, nan } },
        { {  1,  4, -8,  7 }, { -5.6389f, nan, nan } },
        { {  1,  3, -6, -8 }, { -4, -1, 2, } },
    };

    for (auto &i: CUBICS) {
        auto s = util::polynomial::solve (i.coeffs);

        std::sort (s.begin (), s.end ());

        for (size_t j = 0; j < 3; ++j) {
            bool equal = ieee_single::almost_equal (i.solutions[j], s[j]);
            bool invalid = std::isnan (i.solutions[j]) && std::isnan (s[j]);

            CHECK (equal || invalid);
        }
    }
}
