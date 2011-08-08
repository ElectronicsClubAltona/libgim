/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2011 Danny Robson <danny@blubinc.net>
 */

#include "fixed.hpp"

#include <cmath>

/*
 * Constructors
 */
template <unsigned int INT, unsigned int FRAC>
fixed<INT, FRAC>::fixed (integral_type val):
    m_value (val << FRAC)
{
    static_assert (INT  > 0, "must use positive integer bits");
    static_assert (FRAC > 0, "must use positive fractional bits");
}


/*
 * Conversions
 */

template <unsigned int INT, unsigned int FRAC>
double
fixed<INT, FRAC>::to_double (void) const
    { return m_value / std::pow (2.0, FRAC); }


template <unsigned int INT, unsigned int FRAC>
float
fixed<INT, FRAC>::to_float (void) const
    { return m_value / std::pow (2.0f, FRAC); }


template <unsigned int INT, unsigned int FRAC>
typename fixed<INT, FRAC>::integral_type
fixed<INT, FRAC>::to_integral (void) const
    { return m_value >> FRAC; }


/*
 * Integral operators
 */

template <unsigned int INT, unsigned int FRAC>
fixed<INT, FRAC>&
fixed<INT, FRAC>::operator +=(integral_type val) {
    m_value += val << FRAC;
    return *this;
}


template <unsigned int INT, unsigned int FRAC>
fixed<INT, FRAC>&
fixed<INT, FRAC>::operator -=(integral_type val) {
    m_value -= val << FRAC;
    return *this;
}


template <unsigned int INT, unsigned int FRAC>
fixed<INT, FRAC>&
fixed<INT, FRAC>::operator *=(integral_type val) {
    m_value *= val;
    return *this;
}


template <unsigned int INT, unsigned int FRAC>
fixed<INT, FRAC>&
fixed<INT, FRAC>::operator /=(integral_type val) {
    m_value /= val;
    return *this;
}


template <unsigned int INT, unsigned int FRAC>
fixed<INT, FRAC>
fixed<INT, FRAC>::operator +(integral_type val) const
    { return fixed<INT, FRAC>(m_value + val << FRAC); }


template <unsigned int INT, unsigned int FRAC>
fixed<INT, FRAC>
fixed<INT, FRAC>::operator -(integral_type val) const
    { return fixed<INT, FRAC>(m_value - val << FRAC); }


template <unsigned int INT, unsigned int FRAC>
fixed<INT, FRAC>
fixed<INT, FRAC>::operator *(integral_type val) const
    { return fixed<INT, FRAC>(m_value * val); }


template <unsigned int INT, unsigned int FRAC>
fixed<INT, FRAC>
fixed<INT, FRAC>::operator /(integral_type val) const
    { return fixed<INT, FRAC>(m_value / val); }


template class fixed<16,16>;
template class fixed<32,32>;
