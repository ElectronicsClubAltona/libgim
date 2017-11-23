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

    auto vec = util::vector4f (0.5f);
    tap.expect_eq (vec, util::normalised (vec), "normalisation of normalised vector");

    tap.expect_eq (sum (util::vector4f::ones ()), 4.f, "elementwise summation");

    // check that structured bindings work
    {
        const auto &[x,y] = p;
        tap.expect (x == p.x && y == p.y, "structured bindings extract correct data");
    }

    // ensure the distance function behaves correctly with non-normal numbers.
    {
        util::point3f a { 103, 0, 14 };
        util::point3f b { 104, INFINITY, 15 };

        tap.expect_eq (
            std::numeric_limits<float>::infinity (),
            ::util::distance (a, b),
            "distance with an infinity is infinite"
        );
    }

    // test expected outputs for various vector-logical operations
    {
        constexpr util::point3i a {  0, -1,  2 };
        constexpr util::point3i b {  0,  1, -2 };
        constexpr util::point3i c { -9, -9, -9 };

        tap.expect (!all (a <= b), "all, expected failure");
        tap.expect ( all (a <= a), "all, expected success");
        tap.expect (!any (a <= c), "any, expected failure");
        tap.expect ( any (a <= b), "any, expected success");
    };

    // ensure the util::select function behaves as expected
    {
        const util::point3f a { -1, 2, 0 };
        const util::point3f b {  1, 0, 2 };

        const util::point3f lo { -1, 0, 0 };
        const util::point3f hi {  1, 2, 2 };

        tap.expect_eq (select (a < b, a, b), lo, "select with points and min");
        tap.expect_eq (select (a > b, a, b), hi, "select with points and max");
    };

    // ensure that util::limit resolves to the coord overload. the exact
    // values are less useful than exercising the compiler/linker.
    {
        const util::vector3f val {  0, -1,  2 };
        const util::vector3f lo  { -1,  1, -2 };
        const util::vector3f hi  {  1,  2,  0 };

        tap.expect_eq (limit (val,  lo,  hi), util::vector3f { 0, 1, 0 }, "limit with vec/vec/vec");
        tap.expect_eq (limit (val, 0.f,  hi), util::vector3f { 0, 0, 0 }, "limit with vec/num/vec");
        tap.expect_eq (limit (val,  lo, 2.f), util::vector3f { 0, 1, 2 }, "limit with vec/vec/num");
        tap.expect_eq (limit (val, 0.f, 2.f), util::vector3f { 0, 0, 2 }, "limit with vec/num/num");
    }

    // ensure that klass::indices appears to link correctly
    {
        const util::vector3i seq { 0, 1, 2 };
        const util::vector4i res { 2, 0, 0, 1 };

        tap.expect_eq (seq.indices<2,0,0,1> (), res, "coord::indices expansion");
    };

    // ensure that util::shift operations appear to operate correctly
    {
        const util::vector3i seq { 0, 1, 2 };
        tap.expect_eq (rshift (seq, 1, 0), util::make_vector (0, 0, 1), "rshift, scalar fill");

        tap.expect_eq (
            rshift (seq, 2, util::make_vector (3, 4, 5 )),
            util::make_vector (3, 4, 0),
            "rshift, coord fill"
        );
    };

    return tap.status ();
}
