#include "colour.hpp"
 
#include "debug.hpp"

int
main (int, char**)
{
    CHECK_EQ (&util::colour4f::WHITE.x,
              &util::colour4f::WHITE.r);

    CHECK_EQ (util::colour4f::WHITE.x, 1.f);
    CHECK_EQ (util::colour4f::WHITE.r, 1.f);
    CHECK_EQ (util::colour4f::WHITE.g, 1.f);
    CHECK_EQ (util::colour4f::WHITE.b, 1.f);
    CHECK_EQ (util::colour4f::WHITE.a, 1.f);
}
