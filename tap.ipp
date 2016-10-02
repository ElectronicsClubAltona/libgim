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

#include <algorithm>
#include <cstdlib>
#include <iostream>

#include "format.hpp"


///////////////////////////////////////////////////////////////////////////////
template <typename ...Args, size_t N>
void
util::TAP::logger::expect (bool test, const char (&fmt)[N], Args&&... args)
{
    std::cout << (test ? "ok " : "not ok ") << ++m_size
              << " - "
              << util::format::render (fmt, std::forward<Args> (args)...) << '\n';

    if (!test)
        m_status = EXIT_FAILURE;
}


//-----------------------------------------------------------------------------
template <typename ...Args, size_t N>
void
util::TAP::logger::expect (const std::function<bool(Args...)> &test, Args&&... args, const char (&fmt)[N])
{
    try {
        expect (test (std::forward<Args> (args)...), fmt);
    } catch (...) {
        expect (false, fmt);
    }
}


///////////////////////////////////////////////////////////////////////////////
template <typename T, typename U, typename ...Args, size_t N>
void
util::TAP::logger::expect_eq (const T&a, const U &b, const char (&fmt)[N], Args&&... args)
{
    expect (almost_equal (a, b), fmt, std::forward<Args> (args)...);
}


//-----------------------------------------------------------------------------
template <typename T, typename U, typename ...Args, size_t N>
void
util::TAP::logger::expect_neq (const T&a, const U &b, const char (&fmt)[N], Args&&... args)
{
    expect (!almost_equal (a, b), fmt, std::forward<Args> (args)...);
}


///////////////////////////////////////////////////////////////////////////////
#define TAP_TEST(SUFFIX,OP)                                     \
template <typename T, typename U, typename ...Args, size_t N>   \
void                                                            \
util::TAP::logger::expect_ ## SUFFIX (const T &a,               \
                                      const U &b,               \
                                      const char (&fmt)[N],     \
                                      Args&&... args)           \
{                                                               \
    expect ((a) OP (b), fmt, std::forward<Args> (args)...);     \
}

TAP_TEST(gt, > )
TAP_TEST(ge, >=)
TAP_TEST(lt, < )
TAP_TEST(le, <=)

#undef TAP_TEST


//-----------------------------------------------------------------------------
template <typename T, typename ...Args, size_t N>
void
util::TAP::logger::expect_nan (const T &t, const char (&fmt)[N], Args&&... args)
{
    expect (std::isnan (t), fmt, std::forward<Args> (args)...);
}


//-----------------------------------------------------------------------------
template <typename T, typename ...Args, size_t N>
void
util::TAP::logger::expect_nothrow (T &&t, const char (&fmt)[N], Args&&... args)
{
    bool success = true;

    try {
        t ();
    } catch (...) {
        success = false;
    }

    expect (success, fmt, std::forward<Args> (args)...);
}


//-----------------------------------------------------------------------------
template <typename E, typename T, typename ...Args, size_t N>
void
util::TAP::logger::expect_throw (T &&t, const char (&fmt)[N], Args&&... args)
{
    bool success = false;

    try {
        t ();
    } catch (const E&) {
        success = true;
    } catch (...) {
        success = false;
    }

    expect (success, fmt, std::forward<Args> (args)...);
}
