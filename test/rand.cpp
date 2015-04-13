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
test_bool (void) {
    static const unsigned ITERATIONS = 8192;
    static const unsigned THRESHOLD  = ITERATIONS / 10;

    unsigned counts[2] = { 0, 0 };
    for (unsigned i = 0; i < ITERATIONS; ++i)
        ++counts[util::random<bool> () ? 0 : 1];

    unsigned diff = counts[0] > counts[1] ?
                    counts[0] - counts[1] :
                    counts[1] - counts[0];

    CHECK_LT (diff, THRESHOLD);
}


// TODO: Use a more robust test like Kolmogorov-Smirnov
void
test_float (void) {
    static const unsigned BUCKETS    =    8;
    static const unsigned ITERATIONS = 8912;
    static const unsigned EXPECTED   = ITERATIONS / BUCKETS;
    static const float    THRESHOLD  = EXPECTED / 10;

    unsigned counts[BUCKETS] = { 0 };
    for (unsigned i = 0; i < ITERATIONS; ++i)
        ++counts[unsigned (util::random<float> () * BUCKETS)];

    for (unsigned c: counts) {
        unsigned diff = EXPECTED > c ?
                        EXPECTED - c :
                        c - EXPECTED;

        CHECK_LT (diff, THRESHOLD);
    }
}


int
main (int, char **) {
    srand (time (NULL));
    test_bool ();
    test_float ();

    util::TAP::logger tap;
    tap.skip ("convert to TAP");
}
