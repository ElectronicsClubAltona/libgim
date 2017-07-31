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

    util::TAP::logger tap;

    constexpr util::point2i p { -1,  2 };

    tap.expect_eq (-p, util::point2i { 1, -2 }, "unary point negation");
    tap.expect_eq ( p, p, "unary point addition");
    tap.expect (
        std::is_same<
            bool,
            decltype(!p)::value_type
        >::value,
        "unary point boolean negation has type bool"
    );

    auto vec = util::vector4f (0.5f);
    tap.expect_eq (vec, util::normalised (vec), "normalisation of normalised vector");

    tap.expect_eq (sum (util::vector4f::ones ()), 4.f, "elementwise summation");

    // check that structured bindings work
    {
        const auto &[x,y] = p;
        tap.expect (x == p.x && y == p.y, "structured bindings extract correct data");
    }

    {
        util::point3f a { 103, 0, 14 };
        util::point3f b { 104, INFINITY, 15 };

        tap.expect_eq (
            std::numeric_limits<float>::infinity (),
            ::util::distance (a, b),
            "distance with an infinity is infinite"
        );
    }

    {
        const util::point3f a { -1, 2, 0 };
        const util::point3f b {  1, 0, 2 };

        const util::point3f lo { -1, 0, 0 };
        const util::point3f hi {  1, 2, 2 };

        tap.expect_eq (select (a < b, a, b), lo, "select with points and min");
        tap.expect_eq (select (a > b, a, b), hi, "select with points and max");
    };

    return tap.status ();
}
