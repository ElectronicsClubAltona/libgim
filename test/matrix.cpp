#include "matrix.hpp"

#include "debug.hpp"
#include "tap.hpp"
#include "vector.hpp"

#include <cstdlib>

int
main (void)
{
    util::TAP::logger tap;

    {
        // Identity matrix-vector multiplication
        auto v = util::vector<4,float> { 1.f, 2.f, 3.f, 4.f };
        auto r = util::matrix<float>::IDENTITY * v;
        tap.expect_eq (r, v, "identity matrix-vector multiplication");
    }

    {
        // Simple matrix-vector multiplication
        util::matrix<float> m { {
            {  1,  2,  3,  4 },
            {  5,  6,  7,  8 },
            {  9, 10, 11, 12 },
            { 13, 14, 15, 16 }
        } };

        util::vector<4,float> v { 1.f, 2.f, 3.f, 4.f };

        auto r = m * v;

        tap.expect (
            almost_equal (r.x,  30) &&
            almost_equal (r.y,  70) &&
            almost_equal (r.z, 110) &&
            almost_equal (r.w, 150),
            "simple matrix-vector multiplication"
        );
    }

    {
        // Simple matrix-matrix multiplication
        util::matrix<float> a { {
            {  1,  2,  3,  4 },
            {  5,  6,  7,  8 },
            {  9, 10, 11, 12 },
            { 13, 14, 15, 16 },
        } };

        util::matrix<float> b { {
            { 17, 18, 19, 20 },
            { 21, 22, 23, 24 },
            { -1, -2, -3, -4 },
            { -5, -6, -7, -8 }
        } };

        util::matrix<float> ab { {
            {   9,   8,   7,   6 },
            {  41,  40,  39,  38 },
            {  73,  72,  71,  70 },
            { 105, 104, 103, 102 },
        } };

        ab *= 4;

        auto res = a * b;

        tap.expect_eq (ab, res, "simple matrix-matrix multiplication");
    }


    {
        bool success = true;

        // Ensure identity inverts to identity
        auto m = util::matrix<float>::IDENTITY.inverse ();
        for (size_t r = 0; r < m.rows; ++r)
            for (size_t c = 0; c < m.cols; ++c)
                if (r == c)
                    success = success && almost_equal (m.values[r][c], 1);
                else
                    success = success && almost_equal (m.values[r][c], 0);

        tap.expect (success, "identity inversion");
    }

    {
        // Simple inversion test
        util::matrix<float> m { {
            { 4, 1, 2, 3 },
            { 2, 3, 4, 1 },
            { 3, 4, 1, 2 },
            { 1, 2, 3, 4 }
        } };

        util::matrix<float> r { {
            { 11,  1,  1, -9 },
            { -9,  1, 11,  1 },
            {  1, 11, -9,  1 },
            {  1, -9,  1, 11 }
        } };

        tap.expect_eq (m.inverse (), r / 40.f, "simple inversion");
    }

    return tap.status ();
}
