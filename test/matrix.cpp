#include "../debug.hpp"
#include "../matrix.hpp"
#include "../maths.hpp"

#include <iostream>
#include <cmath>

using namespace maths;
using namespace std;


std::ostream&
operator <<(std::ostream &os, const matrix &m) {
    for (unsigned int i = 0; i < m.rows (); ++i) {
        for (unsigned int j = 0; j < m.columns (); ++j) {
            os << m[i][j];
            if (j != m.columns () - 1)
                os << ", ";
        }

        if (i != m.rows () - 1)
            os << "\n";
    }

    return os;
}


void 
test_zeroes (const matrix &m) {
    assert (m.rows ());
    assert (m.columns ());

    for (unsigned int i = 0; i < m.rows (); ++i) 
        for (unsigned int j = 0; j < m.columns (); ++j)
            check_hard (almost_equal (m[i][j], 0.0));
}


void
test_identity (const matrix &m) {
    assert (m.rows () == m.columns ());

    for (unsigned int i = 0; i < m.rows (); ++i)
        for (unsigned int j = 0; j < m.columns (); ++j)
            if (i == j)
                check_hard (almost_equal (m[i][j], 1.0));
            else
                check_hard (almost_equal (m[i][j], 0.0));
}


int
main (int, char **) {
    for (unsigned int i = 1; i < 10; ++i) {
        test_zeroes   (matrix::zeroes   (i));
        test_identity (matrix::identity (i));
    }

    for (unsigned int i = 3; i < 10; i += 2)
        check (matrix::magic (i).is_magic ());

    // Create a small matrix with unique element values for comparison tests.
    // This should be non-square so that row- vs. column-major problems can
    // be seen.
    matrix a4x2 (4, 2, { 0, 1,
                      2, 3,
                      4, 5,
                      6, 7 });
    check_eq (a4x2, a4x2);

    // Test that copy constructors work correctly. Keep this value around so
    // that we can check the following operators don't modify the original
    // value.
    check_eq (a4x2, matrix(a4x2));

    // Check multiplication by identity results in the original value.
    check_eq (a4x2, a4x2 * matrix::identity (a4x2.columns ()));

    matrix seq2x2(2, 2, { 1, 2, 3, 4 });
    matrix magic3(3, 3, { 2, 7, 6,
                          9, 5, 1,
                          4, 3, 8 });

    matrix magic4(4, 4, { 16,  2,  3, 13,
                           5, 11, 10,  8,
                           9,  7,  6, 12,
                           4, 14, 15,  1 });


    check_eq (magic3[0][0], 2.0);
    check_eq (magic3[0][1], 7.0);
    check_eq (magic3[0][2], 6.0);
    check_eq (magic3[1][0], 9.0);
    check_eq (magic3[1][1], 5.0);
    check_eq (magic3[1][2], 1.0);
    check_eq (magic3[2][0], 4.0);
    check_eq (magic3[2][1], 3.0);
    check_eq (magic3[2][2], 8.0);
    
    check_eq (seq2x2.determinant (),   -2.0);
    check_eq (magic3.determinant (), -360.0);

    check_hard ( seq2x2.is_square ());
    check_hard ( magic3.is_square ());
    check_hard (!  a4x2.is_square ());

    check_eq (seq2x2.inverse (), matrix (2, 2, { -2.0,  1.0,
                                                  1.5, -0.5 }));
    check_eq (magic3.inverse (), matrix (3, 3, { -37.0,  38.0,  23.0,
                                                  68.0,   8.0, -52.0,
                                                 - 7.0, -22.0,  53.0 }) /= 360.0);

    matrix invertible4 (4, 4, { 4, 14, 15,  1,
                                9,  7,  6, 12, 
                                5, 11, 10,  8,
                                0,  0,  0,  1 });
    check_eq (invertible4.inverse (), matrix (4, 4, {   4,  25, -21, -136,
                                                      -60, -35, 111, -408,
                                                       64,  26, -98,  408,
                                                        0,   0,   0,  136 }) /= 136);

    const matrix homo3x3 (3, 3, { 1, 2, 0,
                                  3, 4, 0,
                                  0, 0, 1 });
    check_hard (homo3x3.is_homogeneous ());
    check_hard (!matrix::zeroes   (3).is_homogeneous ());
    check_hard ( matrix::identity (3).is_homogeneous ());
    check_hard (invertible4.is_homogeneous ());

    return EXIT_SUCCESS;
}
