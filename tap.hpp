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

#ifndef __UTIL_TAP_HPP
#define __UTIL_TAP_HPP

#include <iostream>

#define TAP_PLAN(N) do {                                            \
    std::cout << "1.." << (N) << '\n';                              \
} while (0)


#define TAP(OK, MSG) do {                                           \
    std::cout << ((OK) ? "ok - " : "not ok - ") << (MSG) << '\n';   \
} while (0)


#define TAP_EQ(A,B,MSG) do {                                        \
    const auto &&__tap_eq_a = (A);                                  \
    const auto &&__tap_eq_b = (B);                                  \
                                                                    \
    TAP (almost_equal (__tap_eq_a, __tap_eq_b), (MSG));             \
} while (0)


#define TAP_SKIP(MSG) do {                                          \
    std::cout << "ok # skip " << (MSG) << '\n';                     \
} while (0)


#define TAP_BAIL(MSG) do {                                          \
    std::cout << "Bail out! " << (MSG) << '\n';                     \
} while (0)

#endif
