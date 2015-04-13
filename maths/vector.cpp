#include "vector.hpp"

#include "debug.hpp"

#include <numeric>


using namespace maths;


/* Constructors */

vector::vector (const std::initializer_list<double> &_data):
    m_data (_data)
{ ; }


vector::vector (unsigned int           _size):
    m_data (_size)
{ ; }


vector::vector (const double *restrict _data,
                unsigned int           _size):
    m_data (_size)
{ std::copy (_data, _data + _size, m_data.begin ()); }


vector::vector (const vector &rhs):
    m_data (rhs.m_data)
{ ; }


vector::vector (const vector &&rhs):
    m_data (std::move (rhs.m_data))
{ ; }


vector::~vector (void)
{ ; }


/* element accessors */

const double*
vector::data (void) const
    { return &m_data[0]; }


double &
vector::operator[] (unsigned int offset)
    { return m_data[offset]; }


const double&
vector::operator[] (unsigned int offset) const
    { return m_data[offset]; }


unsigned int
vector::size (void) const
    { return m_data.size (); }


/* dot and cross products */

double vector::dot (const double *restrict A,
                    const double *restrict B,
                    unsigned int size)
{ return std::inner_product(A, A + size, B, 0.0); }


vector vector::cross (const double *restrict A,
                      const double *restrict B,
                      unsigned int size) {
    CHECK_EQ (size, 3);
    (void)size;
    return vector ({ A[1] * B[2] - A[2] * B[1],
                     A[2] * B[0] - A[0] * B[2],
                     A[0] * B[1] - A[1] * B[0] });
}
