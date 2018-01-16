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
 * Copyright 2017 Danny Robson <danny@nerdcruft.net>
 */

#include "parse.hpp"

#include "cast.hpp"

#include <cstdlib>
#include <stdexcept>

using util::parse;

///////////////////////////////////////////////////////////////////////////////
template <typename T> struct c_traits;
template <> struct c_traits<long> { static constexpr auto func = strtol; };
template <> struct c_traits<long long> { static constexpr auto func = strtoll; };
template <> struct c_traits<unsigned long> { static constexpr auto func = strtoul; };
template <> struct c_traits<unsigned long long> { static constexpr auto func = strtoull; };

template <> struct c_traits<float> { static constexpr auto func = strtof; };
template <> struct c_traits<double> { static constexpr auto func = strtod; };
template <> struct c_traits<long double> { static constexpr auto func = strtold; };


//-----------------------------------------------------------------------------
template <typename T>
T
c_iparse (const char *first, const char *last)
{
    auto tail = const_cast<char *> (last);
    auto val = c_traits<T>::func (first, &tail, 0);

    if (tail != last)
        throw std::invalid_argument ("unable to parse");

    return val;
}


//-----------------------------------------------------------------------------
template <typename T>
T
c_fparse (const char *first, const char *last)
{
    auto tail = const_cast<char *> (last);
    auto val = c_traits<T>::func (first, &tail);

    if (tail != last)
        throw std::invalid_argument ("unable to parse");

    return val;
}




///////////////////////////////////////////////////////////////////////////////
#define C_PARSE(T, KLASS) \
template <> \
T \
util::parse<T> (util::view<const char *> str) \
{ \
    return c_ ## KLASS ## parse<T> (std::cbegin (str), std::cend (str)); \
}


//-----------------------------------------------------------------------------
C_PARSE(long, i)
C_PARSE(long long, i)
C_PARSE(unsigned long, i)
C_PARSE(unsigned long long, i)

C_PARSE(float, f)
C_PARSE(double, f)
C_PARSE(long double, f)


template <>
int
util::parse<int> (util::view<const char*> str)
{
    auto intermediate = util::parse<long> (str);
    return util::cast::lossless<int> (intermediate);
}
