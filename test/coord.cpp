#include "tap.hpp"
#include "point.hpp"
#include "vector.hpp"

#include "coord/iostream.hpp"

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

    constexpr util::point2i p { -1,  2 };

    t.expect_eq (-p, util::point2i { 1, -2 }, "unary point negation");
    t.expect_eq ( p, p, "unary point addition");
    t.expect (
        std::is_same<
            bool,
            decltype(!p)::value_type
        >::value,
        "unary point boolean negation has type bool"
    );

    auto vec = util::vector4f (0.5f);
    t.expect_eq (vec, util::normalised (vec), "normalisation of normalised vector");

    return t.status ();
}
