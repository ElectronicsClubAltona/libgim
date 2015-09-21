#include "tap.hpp"
#include "point.hpp"

int
main (void)
{
    static_assert (sizeof (util::point1u) == 1 * sizeof (util::point1u::value_type), "point1u is not packed");
    static_assert (sizeof (util::point2u) == 2 * sizeof (util::point2u::value_type), "point2u is not packed");
    static_assert (sizeof (util::point3u) == 3 * sizeof (util::point3u::value_type), "point3u is not packed");
    static_assert (sizeof (util::point4u) == 4 * sizeof (util::point4u::value_type), "point4u is not packed");

    static_assert (sizeof (util::point1f) == 1 * sizeof (util::point1f::value_type), "point1f is not packed");
    static_assert (sizeof (util::point2f) == 2 * sizeof (util::point2f::value_type), "point2f is not packed");
    static_assert (sizeof (util::point3f) == 3 * sizeof (util::point3f::value_type), "point3f is not packed");
    static_assert (sizeof (util::point4f) == 4 * sizeof (util::point4f::value_type), "point4f is not packed");

    util::TAP::logger t;
    t.expect (true, "packing tests");

    return t.status ();
}
