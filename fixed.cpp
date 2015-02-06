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
 * Copyright 2011-2015 Danny Robson <danny@nerdcruft.net>
 */

#include "fixed.hpp"

#include "maths.hpp"

#include <cmath>

using namespace util;

///////////////////////////////////////////////////////////////////////////////
// Constructors
template <unsigned I, unsigned E>
fixed<I,E>::fixed (uint_t val):
    m_value (val << E)
{
    static_assert (I > 0, "must use positive integer bits");
    static_assert (E > 0, "must use positive fractional bits");
    static_assert (I + E == sizeof (m_value) * 8,
                   "underlying storage must be exactly I+E sized");
}


///////////////////////////////////////////////////////////////////////////////
// Conversions

template <unsigned I, unsigned E>
double
fixed<I,E>::to_double (void) const
{
    return static_cast<double> (m_value) / pow (2, E);
}


//-----------------------------------------------------------------------------
template <unsigned I, unsigned E>
float
fixed<I,E>::to_float (void) const
{
    return static_cast<float> (m_value) / pow (2, E);
}


//-----------------------------------------------------------------------------
template <unsigned I, unsigned E>
typename fixed<I,E>::uint_t
fixed<I,E>::to_integral (void) const
{
    return m_value >> E;
}


//-----------------------------------------------------------------------------
template <unsigned I, unsigned E>
typename fixed<I,E>::uint_t
fixed<I,E>::to_native (void) const
{
    return m_value;
}


//-----------------------------------------------------------------------------
template <unsigned I, unsigned E>
fixed<I,E>
fixed<I,E>::from_native (uint_t i)
{
    return fixed<I,E> {i};
}


//-----------------------------------------------------------------------------
template <unsigned I, unsigned E>
typename fixed<I,E>::uint_t
fixed<I,E>::to_integral (uint_t v)
{
    return v >> E;
}


///////////////////////////////////////////////////////////////////////////////
// Integer operators

template <unsigned I, unsigned E>
fixed<I,E>&
fixed<I,E>::operator+= (uint_t val)
{
    m_value += val << E;
    return *this;
}


//-----------------------------------------------------------------------------
template <unsigned I, unsigned E>
fixed<I,E>&
fixed<I,E>::operator-= (uint_t val)
{
    m_value -= val << E;
    return *this;
}


//-----------------------------------------------------------------------------
template <unsigned I, unsigned E>
fixed<I,E>&
fixed<I,E>::operator*= (uint_t val)
{
    m_value *= val;
    return *this;
}


//-----------------------------------------------------------------------------
template <unsigned I, unsigned E>
fixed<I,E>&
fixed<I,E>::operator/= (uint_t val)
{
    m_value /= val;
    return *this;
}


//-----------------------------------------------------------------------------
template <unsigned I, unsigned E>
fixed<I,E>
fixed<I,E>::operator+ (uint_t val) const
{
    return fixed<I,E> (m_value + val << E);
}


//-----------------------------------------------------------------------------
template <unsigned I, unsigned E>
fixed<I,E>
fixed<I,E>::operator- (uint_t val) const
{
    return fixed<I,E> (m_value - val << E);
}


//-----------------------------------------------------------------------------
template <unsigned I, unsigned E>
fixed<I,E>
fixed<I,E>::operator* (uint_t val) const
{
    return fixed<I,E> (m_value * val);
}


//-----------------------------------------------------------------------------
template <unsigned I, unsigned E>
fixed<I,E>
fixed<I,E>::operator /(uint_t val) const
{
    return fixed<I,E> (m_value / val);
}


///////////////////////////////////////////////////////////////////////////////
// logical operators

#define LOGIC_OP(OP)                            \
template <unsigned I, unsigned E>               \
bool                                            \
util::operator OP (util::fixed<I,E> a,          \
             util::fixed<I,E> b)                \
{                                               \
    return a.to_native () OP b.to_native ();    \
}

LOGIC_OP(==)
LOGIC_OP(!=)
LOGIC_OP(<)
LOGIC_OP(<=)
LOGIC_OP(>)
LOGIC_OP(>=)


///////////////////////////////////////////////////////////////////////////////
// Instantiations

#define INSTANTIATE(I,E)                                                        \
template class util::fixed<(I),(E)>;                                            \
template bool util::operator== (util::fixed<(I),(E)>, util::fixed<(I),(E)>);    \
template bool util::operator!= (util::fixed<(I),(E)>, util::fixed<(I),(E)>);    \
template bool util::operator<  (util::fixed<(I),(E)>, util::fixed<(I),(E)>);    \
template bool util::operator<= (util::fixed<(I),(E)>, util::fixed<(I),(E)>);    \
template bool util::operator>  (util::fixed<(I),(E)>, util::fixed<(I),(E)>);    \
template bool util::operator>= (util::fixed<(I),(E)>, util::fixed<(I),(E)>);

INSTANTIATE(16,16)
INSTANTIATE(26, 6)
INSTANTIATE(32,32)
