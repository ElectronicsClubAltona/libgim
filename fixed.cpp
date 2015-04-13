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
 * Copyright 2011-2015 Danny Robson <danny@nerdcruft.net>
 */

#include "fixed.hpp"

#include "maths.hpp"

#include <cmath>

using namespace util;

///////////////////////////////////////////////////////////////////////////////
// Constructors
template <typename T, unsigned I, unsigned E>
fixed<T,I,E>::fixed (native_t val):
    m_value (val << E)
{
    static_assert (I > 0, "must use positive integer bits");
    static_assert (E > 0, "must use positive fractional bits");
    static_assert (I + E == sizeof (m_value) * 8,
                   "underlying storage must be exactly I+E sized");
}


///////////////////////////////////////////////////////////////////////////////
// Conversions

template <typename T, unsigned I, unsigned E>
double
fixed<T,I,E>::to_double (void) const
{
    return static_cast<double> (m_value) / pow (2, E);
}


//-----------------------------------------------------------------------------
template <typename T, unsigned I, unsigned E>
float
fixed<T,I,E>::to_float (void) const
{
    return static_cast<float> (m_value) / pow (2, E);
}


//-----------------------------------------------------------------------------
template <typename T, unsigned I, unsigned E>
typename fixed<T,I,E>::native_t
fixed<T,I,E>::to_integer (void) const
{
    return m_value >> E;
}


//-----------------------------------------------------------------------------
template <typename T, unsigned I, unsigned E>
typename fixed<T,I,E>::native_t
fixed<T,I,E>::to_native (void) const
{
    return m_value;
}


//-----------------------------------------------------------------------------
template <typename T, unsigned I, unsigned E>
fixed<T,I,E>
fixed<T,I,E>::from_native (native_t i)
{
    fixed<T,I,E> v;
    v.m_value = i;
    return v;
}


//-----------------------------------------------------------------------------
template <typename T, unsigned I, unsigned E>
typename fixed<T,I,E>::native_t
fixed<T,I,E>::to_integer (native_t n)
{
    return n >> E;
}


///////////////////////////////////////////////////////////////////////////////
// Fixed operators
#define SIMPLE_FIXED_REF(OP)                                \
template <typename T, unsigned I, unsigned E>               \
util::fixed<T,I,E>&                                         \
util::fixed<T,I,E>::operator OP (const fixed<T,I,E> rhs)    \
{                                                           \
    m_value OP rhs.m_value;                                 \
    return *this;                                           \
}

SIMPLE_FIXED_REF(-=)
SIMPLE_FIXED_REF(+=)


#define SIMPLE_FIXED_LIT(OP)                                    \
template <typename T, unsigned I, unsigned E>                   \
util::fixed<T,I,E>                                              \
util::fixed<T,I,E>::operator OP (const fixed<T,I,E> rhs) const  \
{                                                               \
    fixed<T,I,E> v;                                             \
    v.m_value = m_value OP rhs.m_value;                         \
    return v;                                                   \
}

SIMPLE_FIXED_LIT(-)
SIMPLE_FIXED_LIT(+)


///////////////////////////////////////////////////////////////////////////////
// Integer operators
#define SIMPLE_INTEGER_REF(OP)                  \
template <typename T, unsigned I, unsigned E>   \
fixed<T,I,E>&                                   \
fixed<T,I,E>::operator OP (native_t val)        \
{                                               \
    m_value OP val << E;                        \
    return *this;                               \
}

SIMPLE_INTEGER_REF(+=)
SIMPLE_INTEGER_REF(-=)
SIMPLE_INTEGER_REF(*=)
SIMPLE_INTEGER_REF(/=)


//-----------------------------------------------------------------------------
#define SIMPLE_INTEGER_LIT(OP)                               \
template <typename T, unsigned I, unsigned E>               \
fixed<T,I,E>                                                \
fixed<T,I,E>::operator OP (native_t val) const              \
{                                                           \
    return fixed<T,I,E>::from_native (m_value OP val << E); \
}

SIMPLE_INTEGER_LIT(+)
SIMPLE_INTEGER_LIT(-)
SIMPLE_INTEGER_LIT(*)
SIMPLE_INTEGER_LIT(/)


///////////////////////////////////////////////////////////////////////////////
// logical operators

#define LOGIC_OP(OP)                            \
template <typename T, unsigned I, unsigned E>   \
bool                                            \
util::operator OP (util::fixed<T,I,E> a,        \
                   util::fixed<T,I,E> b)        \
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
// iostream operators
template <typename T, unsigned I, unsigned E>
std::ostream&
util::operator<< (std::ostream &os, fixed<T,I,E> v)
{
    return os << v.to_double ();
}


///////////////////////////////////////////////////////////////////////////////
// Instantiations

#define INSTANTIATE(T,I,E)                                                  \
template class util::fixed<T,I,E>;                                          \
template std::ostream& util::operator<< (std::ostream&, fixed<T,I,E>);      \
template bool util::operator== (util::fixed<T,I,E>, util::fixed<T,I,E>);    \
template bool util::operator!= (util::fixed<T,I,E>, util::fixed<T,I,E>);    \
template bool util::operator<  (util::fixed<T,I,E>, util::fixed<T,I,E>);    \
template bool util::operator<= (util::fixed<T,I,E>, util::fixed<T,I,E>);    \
template bool util::operator>  (util::fixed<T,I,E>, util::fixed<T,I,E>);    \
template bool util::operator>= (util::fixed<T,I,E>, util::fixed<T,I,E>);

template class util::fixed<signed,8,8>;

INSTANTIATE(signed,16,16)
INSTANTIATE(signed,26, 6)
INSTANTIATE(signed,32,32)
INSTANTIATE(unsigned,16,16)
INSTANTIATE(unsigned,26, 6)
INSTANTIATE(unsigned,32,32)
