#include "random.hpp"
#include "debug.hpp"
#include "tap.hpp"

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;


// TODO: Use a more robust test like Chi-Square
void
test_bool (util::TAP::logger &tap)
{
    static const unsigned ITERATIONS = 8192;
    static const unsigned THRESHOLD  = ITERATIONS / 10;

    unsigned counts[2] = { 0, 0 };
    for (unsigned i = 0; i < ITERATIONS; ++i)
        ++counts[util::random<bool> () ? 0 : 1];

    unsigned diff = counts[0] > counts[1] ?
                    counts[0] - counts[1] :
                    counts[1] - counts[0];

    tap.expect_lt (diff, THRESHOLD, "approximately even bool distribution");
}


// TODO: Use a more robust test like Kolmogorov-Smirnov
void
test_float (util::TAP::logger &tap)
{
    static const unsigned BUCKETS    =    8;
    static const unsigned ITERATIONS = 8912;
    static const unsigned EXPECTED   = ITERATIONS / BUCKETS;
    static const float    THRESHOLD  = EXPECTED / 10;

    unsigned counts[BUCKETS] = { 0 };
    for (unsigned i = 0; i < ITERATIONS; ++i)
        ++counts[unsigned (util::random<float> () * BUCKETS)];

    bool success = true;
    for (unsigned c: counts) {
        unsigned diff = EXPECTED > c ?
                        EXPECTED - c :
                        c - EXPECTED;

        success = success && diff < THRESHOLD;
    }

    tap.expect (success, "approximately equal float buckets");
}


int
main (int, char **) {
    util::TAP::logger tap;

    srand (time (NULL));

    test_bool (tap);
    test_float (tap);

    return tap.status ();
}
