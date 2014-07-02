#include "float.hpp"

#include <cmath>

using namespace std;


/* Constructors */

template <unsigned int E, unsigned int S>
ieee_float<E, S>::ieee_float (void)
{ ; }


template <unsigned int E, unsigned int S>
ieee_float<E, S>::ieee_float (floating_t _floating):
    m_floating (_floating)
{ ; }


template <unsigned int E, unsigned int S>
ieee_float<E, S>::ieee_float (const ieee_float &rhs): 
    m_bits (rhs.m_bits)
{ ; }


/* Classifiers */

template <unsigned int E, unsigned int S>
bool
ieee_float<E, S>::is_zero (void) const {
    return m_components.exponent    == 0 &&
           m_components.significand == 0;
}

template <unsigned int E, unsigned int S>
bool
ieee_float<E, S>::is_subnormal (void) const {
    return m_components.exponent    == 0 &&
           m_components.significand != 0;
}

template <unsigned int E, unsigned int S>
bool
ieee_float<E, S>::is_inifinity (void) const {
    return m_components.exponent    == (1 << EXPONENT_BITS) - 1 &&
           m_components.significand == 0;
}

template <unsigned int E, unsigned int S>
bool
ieee_float<E, S>::is_nan (void) const {
    return m_components.exponent    == (1 << EXPONENT_BITS) - 1 &&
           m_components.significand != 0;
}

template <unsigned int E, unsigned int S>
bool
ieee_float<E, S>::operator==(floating_t _floating) const { 
    // TODO: This method really shouldn't be generated if there's no
    // representative native floating point type. But I'm sick of
    // C++'s template bullshit for tonight.
    CHECK_HARD (bits_type<TOTAL_BITS>::has_floating);

    union {
        floating_t _floating;
        uint_t     _uint;
    } convertor;

    convertor._floating = _floating;

    return m_bits == convertor._uint;
}


#include <iostream>


template <unsigned int E, unsigned int S>
bool
ieee_float<E, S>::almost_equal (floating_t a,
                                floating_t b) { 
    // Static cast to avoid integer casting warnings when using uint16_t for half
    static const floating_t epsilon = static_cast<floating_t> (0.001);
    const floating_t diff = static_cast<floating_t> (std::fabs (a - b));

    // * Use an exact equality first so that infinities are not indirectly compared. This would generate NaNs in the diff.
    // * Do not use gte or lte. This stops an infinite from making infinities on both sides of the inequality.
    return exactly_equal (a, b)           ||
           diff < epsilon * std::fabs (a) ||
           diff < epsilon * std::fabs (b);
}


template class ieee_float< 5,  10>; // ieee_half
template class ieee_float< 8,  23>; // ieee_single;
template class ieee_float<11,  52>; // ieee_double;
