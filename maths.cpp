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
 * Copyright 2010-2014 Danny Robson <danny@nerdcruft.net>
 */

#include "maths.hpp"

#include "debug.hpp"

#include "float.hpp"

#include <cmath>
#include <type_traits>


///////////////////////////////////////////////////////////////////////////////
template <typename T>
bool
is_pow2 (T value) {
    typedef typename std::enable_if<std::is_integral<T>::value, bool>::type return_type;
    return (return_type)(value && !(value & (value - 1)));
}


template bool is_pow2 (uint8_t);
template bool is_pow2 (uint16_t);
template bool is_pow2 (uint32_t);
template bool is_pow2 (uint64_t);


///////////////////////////////////////////////////////////////////////////////
template <typename T>
T
log2up (T v)
{
    return log2 ((v << 1) - 1);
}

template uint32_t log2up (uint32_t);
template uint64_t log2up (uint64_t);


///////////////////////////////////////////////////////////////////////////////
template <typename T>
T
log2 (T v) {
    static_assert (std::is_integral<T>::value,
                   "log2 is only implemented for integers");

    T l = 0;
    while (v >>= 1)
        ++l;

    return l;
}

template uint8_t  log2 (uint8_t);
template uint16_t log2 (uint16_t);
template uint32_t log2 (uint32_t);
template uint64_t log2 (uint64_t);


///////////////////////////////////////////////////////////////////////////////
template <typename T>
double
rootsquare (T a, T b)
    { return sqrt (util::pow2 (a) + util::pow2 (b)); }


//-----------------------------------------------------------------------------
template double rootsquare (double, double);
template double rootsquare (   int,    int);


///////////////////////////////////////////////////////////////////////////////
template <typename T>
bool
is_integer (const T &value) {
    T integer;
    return exactly_equal (std::modf (value, &integer),
                          static_cast<T> (0.0));
}


//-----------------------------------------------------------------------------
template bool is_integer (const double&);
template bool is_integer (const  float&);


///////////////////////////////////////////////////////////////////////////////
template <>
unsigned
digits (const uint32_t &v) {
    return (v >= 1000000000) ? 10 :
           (v >=  100000000) ?  9 :
           (v >=   10000000) ?  8 :
           (v >=    1000000) ?  7 :
           (v >=     100000) ?  6 :
           (v >=      10000) ?  5 :
           (v >=       1000) ?  4 :
           (v >=        100) ?  3 :
           (v >=         10) ?  2 :
                                1;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
T
round_pow2 (T value) {
    typedef typename std::enable_if<std::is_integral<T>::value, T>::type return_type;

    --value;

    for (unsigned i = 1; i < sizeof (T) * 8; i <<= 1) {
        value |= value >> i;
    }

    ++value;
    return (return_type)value;
}


//-----------------------------------------------------------------------------
template uint8_t  round_pow2 (uint8_t);
template uint16_t round_pow2 (uint16_t);
template uint32_t round_pow2 (uint32_t);
template uint64_t round_pow2 (uint64_t);


///////////////////////////////////////////////////////////////////////////////
template const float  PI<float>;
template const double PI<double>;


///////////////////////////////////////////////////////////////////////////////
// Simple instantiations. Some functions aren't used internally to the library
// so it's easier to instantiate early and check for broken code at library
// build time.

template float limit (float, float, float);

template float  smoothstep (float,  float,  float);
template double smoothstep (double, double, double);
