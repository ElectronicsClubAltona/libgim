/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#include "float.hpp"
#include "debug.hpp"

#include <cmath>


///////////////////////////////////////////////////////////////////////////////
template <unsigned int E, unsigned int S>
ieee_float<E, S>::ieee_float (void)
{ ; }


//-----------------------------------------------------------------------------
template <unsigned int E, unsigned int S>
ieee_float<E, S>::ieee_float (floating_t _floating):
    m_floating (_floating)
{ ; }


//-----------------------------------------------------------------------------
template <unsigned int E, unsigned int S>
ieee_float<E, S>::ieee_float (const ieee_float &rhs):
    m_bits (rhs.m_bits)
{ ; }


///////////////////////////////////////////////////////////////////////////////
template <unsigned int E, unsigned int S>
bool
ieee_float<E, S>::is_zero (void) const
{
    return m_components.exponent    == 0 &&
           m_components.significand == 0;
}


//-----------------------------------------------------------------------------
template <unsigned int E, unsigned int S>
bool
ieee_float<E, S>::is_subnormal (void) const
{
    return m_components.exponent    == 0 &&
           m_components.significand != 0;
}


//-----------------------------------------------------------------------------
template <unsigned int E, unsigned int S>
bool
ieee_float<E, S>::is_inifinity (void) const {
    return m_components.exponent    == (1 << EXPONENT_BITS) - 1 &&
           m_components.significand == 0;
}

//-----------------------------------------------------------------------------
template <unsigned int E, unsigned int S>
bool
ieee_float<E, S>::is_nan (void) const {
    return m_components.exponent    == (1 << EXPONENT_BITS) - 1 &&
           m_components.significand != 0;
}


///////////////////////////////////////////////////////////////////////////////
template <unsigned int E, unsigned int S>
bool
ieee_float<E, S>::operator==(floating_t _floating) const {
    // TODO: This method really shouldn't be generated if there's no
    // representative native floating point type. But I'm sick of
    // C++'s template bullshit for tonight.
    CHECK (bits_type<TOTAL_BITS>::has_floating);

    union {
        floating_t _floating;
        uint_t     _uint;
    } convertor;

    convertor._floating = _floating;

    return m_bits == convertor._uint;
}


///////////////////////////////////////////////////////////////////////////////
template <unsigned int E, unsigned int S>
bool
ieee_float<E, S>::almost_equal (floating_t a,
                                floating_t b)
{
    return almost_equal (a, b, 128u);
}


//-----------------------------------------------------------------------------
// Based on the Cygnus `AlmostEqual2sComplement` function
template <unsigned int E, unsigned int S>
bool
ieee_float<E, S>::almost_equal (floating_t _a,
                                floating_t _b,
                                unsigned ulps)
{
    // Ensure ULPs is small enough that the default NaNs won't compare as
    // equal to anything else.
    CHECK_LE (ulps, 4 * 1024 * 1024u);

    union {
        floating_t f;
        sint_t     s;
        uint_t     u;
    } a, b;

    a.f = _a;
    b.f = _b;

    // Special case the NaNs early so simplify diffs
    if (std::isnan (a.f) || std::isnan (b.f))
        return false;

    // Early out, as identity comparisons are reasonably common
    if (a.s == b.s)
        return true;

    // Re-base negative floats to be continuous against +ve/-ve 0
    static const union {
        floating_t f;
        sint_t     s;
    } NEG_ZERO { -floating_t {0} };

    if (a.s < 0)
        a.s = NEG_ZERO.s - a.s;
    if (b.s < 0)
        b.s = NEG_ZERO.s - b.s;

    // Calculate ULP difference, but don't use abs(a.s - b.s) as it may cause
    // signed overflow
    uint_t diff = a.u > b.u ? a.u - b.u : b.u - a.u;
    return diff <= ulps;
}


///////////////////////////////////////////////////////////////////////////////
template class ieee_float< 5,  10>; // ieee_half
template class ieee_float< 8,  23>; // ieee_single;
template class ieee_float<11,  52>; // ieee_double;
