#include "matrix.hpp"

#include "debug.hpp"
#include "tap.hpp"
#include "vector.hpp"
#include "coord/iostream.hpp"
#include "quaternion.hpp"

#include <cstdlib>


///////////////////////////////////////////////////////////////////////////////
int
main (void)
{
    util::TAP::logger tap;

    static constexpr util::matrix4f SEQ { {
        {  1,  2,  3,  4 },
        {  5,  6,  7,  8 },
        {  9, 10, 11, 12 },
        { 13, 14, 15, 16 }
    } };

    tap.expect_eq (sum (SEQ), 136.f, "element summation");

    // tranposition
    {
        static constexpr util::matrix4f QES {{
            {  1,  5,  9, 13 },
            {  2,  6, 10, 14 },
            {  3,  7, 11, 15 },
            {  4,  8, 12, 16 }
        }};
        tap.expect_eq (transposed (SEQ), QES, "transposition");
        tap.expect_eq (transposed (transposed (SEQ)), SEQ, "double tranposition is identity");
    }

    // matrix-scalar operations
    {
        tap.expect_eq (sum (SEQ + 1.f), 152.f, "matrix-scalar addition");
        tap.expect_eq (sum (SEQ - 1.f), 120.f, "matrix-scalar subtraction");
        tap.expect_eq (sum (SEQ * 2.f), 272.f, "matrix-scalar multiplication");
        tap.expect_eq (sum (SEQ / 2.f),  68.f, "matrix-scalar division");
    }

    // Simple matrix-vector multiplication
    {
        // Identity matrix-vector multiplication
        auto v = util::vector4f { 1.f, 2.f, 3.f, 4.f };
        auto r = util::matrix4f::IDENTITY * v;
        tap.expect_eq (r, v, "identity matrix-vector multiplication");
    }

    {
        util::vector<4,float> v { 1.f, 2.f, 3.f, 4.f };

        auto r = SEQ * v;

        tap.expect (
            util::almost_equal (r.x,  30.f) &&
            util::almost_equal (r.y,  70.f) &&
            util::almost_equal (r.z, 110.f) &&
            util::almost_equal (r.w, 150.f),
            "simple matrix-vector multiplication"
        );
    }

    {
        // Simple matrix-matrix multiplication
        util::matrix4f a { {
            {  1,  2,  3,  4 },
            {  5,  6,  7,  8 },
            {  9, 10, 11, 12 },
            { 13, 14, 15, 16 },
        } };

        util::matrix4f b { {
            { 17, 18, 19, 20 },
            { 21, 22, 23, 24 },
            { -1, -2, -3, -4 },
            { -5, -6, -7, -8 }
        } };

        util::matrix4f ab { {
            {   9,   8,   7,   6 },
            {  41,  40,  39,  38 },
            {  73,  72,  71,  70 },
            { 105, 104, 103, 102 },
        } };

        ab *= 4.f;

        auto res = a * b;

        tap.expect_eq (ab, res, "simple matrix-matrix multiplication");
    }


    {
        bool success = true;

        // Ensure identity inverts to identity
        auto m = util::matrix4f::IDENTITY.inverse ();
        for (size_t r = 0; r < m.rows; ++r)
            for (size_t c = 0; c < m.cols; ++c)
                if (r == c)
                    success = success && util::almost_equal (m.values[r][c], 1.f);
                else
                    success = success && util::almost_equal (m.values[r][c], 0.f);

        tap.expect (success, "identity inversion");
    }

    // Simple 2x2 inversion test
    {
        util::matrix2f m { {
            { 1, 2 },
            { 3, 4 }
        } };

        tap.expect_eq (-2.f, m.determinant (), "2x2 determinant");

        util::matrix2f r { {
            { -4,  2 },
            {  3, -1 }
        } };

        tap.expect_eq (r / 2.f, m.inverse (), "2x2 inversion");
    }

    // Simple 3x3 inversion test
    {
        util::matrix3f m { {
            { 3, 1, 2 },
            { 2, 3, 1 },
            { 4, 0, 2 }
        } };

        tap.expect_eq (-6.f, m.determinant (), "3x3 determinant");

        util::matrix3f r { {
            { -6,  2,  5 },
            {  0,  2, -1 },
            { 12, -4, -7 }
        } };

        tap.expect_eq (m.inverse (), r / 6.f, "3x3 inversion");
    }

    // Simple 4x4 inversion test
    {
        util::matrix4f m { {
            { 4, 1, 2, 3 },
            { 2, 3, 4, 1 },
            { 3, 4, 1, 2 },
            { 1, 2, 3, 4 }
        } };

        tap.expect_eq (-160.f, m.determinant (), "4x4 determinant");

        util::matrix4f r { {
            { 11,  1,  1, -9 },
            { -9,  1, 11,  1 },
            {  1, 11, -9,  1 },
            {  1, -9,  1, 11 }
        } };

        tap.expect_eq (m.inverse (), r / 40.f, "4x4 inversion");
    }

    // sanity check euler rotations
    {
        static const struct {
            util::vector3f euler;
            const char *msg;
        }  TESTS[] = {
            { util::vector3f { 0 }, "zeroes" },

            { { 1, 0, 0 }, "x-axis" },
            { { 0, 1, 0 }, "y-axis" },
            { { 0, 0, 1 }, "z-axis" },

            { util::vector3f { 1 }, "ones" },

            { {  3,  5,  7 }, "positive primes" },
            { { -3, -5, -7 }, "negative primes" },
            { {  3, -5,  7 }, "mixed primes" },
        };

        for (auto t: TESTS) {
            constexpr auto PI2 = 2 * util::PI<float>;

            auto matrix = (
                util::quaternionf::angle_axis (t.euler[2], { 0, 0, 1 }) *
                util::quaternionf::angle_axis (t.euler[1], { 0, 1, 0 }) *
                util::quaternionf::angle_axis (t.euler[0], { 1, 0, 0 })
            ).as_matrix ();

            auto euler = to_euler (matrix);
            auto truth = t.euler;

            euler = mod (euler + 4 * PI2, PI2);
            truth = mod (truth + 4 * PI2, PI2);

            tap.expect_eq (truth, euler, "matrix-to-euler, %s", t.msg);
        }
    }

    return tap.status ();
}
