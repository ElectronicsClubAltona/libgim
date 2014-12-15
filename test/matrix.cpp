#include "matrix.hpp"
#include "vector.hpp"
#include "debug.hpp"

#include <cstdlib>

int
main (int, char **) {
    {
        // Identity matrix-vector multiplication
        auto v = util::vector<4> { 1, 2, 3, 4 };
        auto r = util::matrix<float>::IDENTITY * v;
        CHECK_EQ (r, v);
    }

    {
        // Simple matrix-vector multiplication
        util::matrix<float> m { {
            {  1,  2,  3,  4 },
            {  5,  6,  7,  8 },
            {  9, 10, 11, 12 },
            { 13, 14, 15, 16 }
        } };

        util::vector<4> v { 1, 2, 3, 4 };

        auto r = m * v;

        CHECK_EQ (r.x,  30);
        CHECK_EQ (r.y,  70);
        CHECK_EQ (r.z, 110);
        CHECK_EQ (r.w, 150);
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

        std::cout << a << "\nx\n" << b << "\n=\n" << (res / 4) << '\n';

        CHECK_EQ (ab, res);
    }


    {
        // Ensure identity inverts to identity
        auto m = util::matrix<float>::IDENTITY.inverse ();
        for (size_t r = 0; r < m.rows; ++r)
            for (size_t c = 0; c < m.cols; ++c)
                if (r == c)
                    CHECK_EQ (m.values[r][c], 1);
                else
                    CHECK_EQ (m.values[r][c], 0);
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

        CHECK_EQ (m.inverse (), r / 40.f);
    }

    return EXIT_SUCCESS;
}
