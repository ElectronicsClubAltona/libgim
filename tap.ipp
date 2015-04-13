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
template <typename T, typename U>
void
util::TAP::logger::expect (std::function<bool(const T&, const U&)> test,
                           const T &a,
                           const U &b,
                           const std::string &msg)
{
    bool success = test (a, b);
    std::cout << (success ? "ok " : "not ok ") << ++m_size << " - " << msg << '\n';

    if (!success)
        m_status = EXIT_FAILURE;
}

//-----------------------------------------------------------------------------
template <typename T, typename U>
void
util::TAP::logger::expect_eq (const T&a, const U &b, const std::string &msg)
{
    expect (almost_equal<T,U>, a, b, msg);
}


//-----------------------------------------------------------------------------
template <typename T, typename U>
void
util::TAP::logger::expect_neq (const T&a, const U &b, const std::string &msg)
{
    static const std::function<bool(const T&, const U&)> TEST = [] (const T &t, const U &u) -> bool {
        return !almost_equal (t, u);
    };

    expect (TEST, a, b, msg);
}


//-----------------------------------------------------------------------------
#define TAP_TEST(SUFFIX,OP)                                     \
template <typename T, typename U>                               \
void                                                            \
util::TAP::logger::expect_ ## SUFFIX (const T &a,               \
                                      const U &b,               \
                                      const std::string &msg)   \
{                                                               \
    std::cout << ((a OP b) ? "ok " : "not ok ") << ++m_size << " - " << msg << '\n'; \
}

TAP_TEST(gt, > )
TAP_TEST(ge, >=)
TAP_TEST(lt, < )
TAP_TEST(le, <=)

#undef TAP_TEST
