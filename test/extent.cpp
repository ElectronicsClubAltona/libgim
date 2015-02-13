#include "extent.hpp"
#include "debug.hpp"

int
main (void)
{
    debug::sanity (util::extent<float> {0, 0});
}
