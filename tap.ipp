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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#ifdef __UTIL_TAP_IPP
#error
#endif
#define __UTIL_TAP_IPP

#include "maths.hpp"

#include <cstdlib>
#include <algorithm>


//-----------------------------------------------------------------------------
template <typename... Args>
void
util::TAP::logger::expect (std::function<bool(Args...)> test, Args&&... args, const std::string &msg)
{
    expect (test (std::forward<Args> (args)...), msg);
}


//-----------------------------------------------------------------------------
template <typename T, typename U>
void
util::TAP::logger::expect_eq (const T&a, const U &b, const std::string &msg)
{
    static const auto TEST = [] (const T &t, const U &u) -> bool {
        return almost_equal (t, u);
    };

    expect<const T&, const U&> (TEST, a, b, msg);
}


//-----------------------------------------------------------------------------
template <typename T, typename U>
void
util::TAP::logger::expect_neq (const T&a, const U &b, const std::string &msg)
{
    static const auto TEST = [] (const T &t, const U &u) -> bool {
        return !almost_equal (t, u);
    };

    expect<const T&, const U&> (TEST, a, b, msg);
}


//-----------------------------------------------------------------------------
#define TAP_TEST(SUFFIX,OP)                                     \
template <typename T, typename U>                               \
void                                                            \
util::TAP::logger::expect_ ## SUFFIX (const T &a,               \
                                      const U &b,               \
                                      const std::string &msg)   \
{                                                               \
    expect<const T&, const U&> ([] (const T&t, const U&u) { return t OP u; }, a, b, msg); \
}

TAP_TEST(gt, > )
TAP_TEST(ge, >=)
TAP_TEST(lt, < )
TAP_TEST(le, <=)

#undef TAP_TEST


//-----------------------------------------------------------------------------
template <typename T>
void
util::TAP::logger::expect_nan (const T &t, const std::string &msg)
{
    bool(*func)(T) = std::isnan;
    expect<const T&> (func, t, msg);
}
