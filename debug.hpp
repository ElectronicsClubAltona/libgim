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
 * Copyright 2010-2012 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __DEBUG_HPP
#define __DEBUG_HPP


#include "annotations.hpp"
#include "maths.hpp"

#include <stdexcept>
#include <iostream>
#include <sstream>

#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define raii CONCATENATE(unique, __COUNTER__)

#define trace {                                                 \
    std::cerr << __FILE__ << ":" << __func__ << ":" << __LINE__ << std::endl;  \
}


#define warn(MSG) do {                                                                      \
   std::cerr << __FILE__ << ":" << __func__ << ":" __LINE__ << ", " << (MSG) << std::endl;  \
} while (0)


#define warn_if(C, MSG) do {                                                                    \
    if (C) {                                                                                    \
        std::cerr << __FILE__ << ":" << __func__ << ":" __LINE__ << ", " << (MSG) << std::endl; \
    }                                                                                           \
} while (0)


#define verify_soft(C, COND) ({     \
    const auto __DEBUG_value = (C); \
    check_soft(__DEBUG_value COND); \
    __DEBUG_value;                  \
})


#define verify_hard(C, COND) ({     \
    const auto __DEBUG_value = (C); \
    check_hard(__DEBUG_value COND); \
    __DEBUG_value;                  \
})


#define _check_meta(C, SUCCESS, FAILURE) do {                       \
    const auto __DEBUG_value = (C);                                 \
    if (unlikely (!__DEBUG_value)) {                                \
        std::cerr <<  PACKAGE << ": "                               \
                  << __FILE__ << ":"                                \
                  << __LINE__ << ": "                               \
                  << __FUNCTION__                                   \
                  << ". Assertion '" << #C                          \
                  << "' failed: "    << __DEBUG_value << std::endl; \
                                                                    \
        { FAILURE }                                                 \
    } else {                                                        \
        { SUCCESS }                                                 \
    }                                                               \
} while (0)


#define check_hard(C) _check_meta((C), { ; }, { panic (); })
#define check_soft(C) _check_meta((C), { ; }, { breakpoint (); })


#define check_eq(A,B) do {                           \
    const auto __a = (A);                            \
    const auto __b = (B);                            \
    _check_meta (almost_equal (__a, __b),            \
                 { ; },                              \
                 {                                   \
        std::ostringstream os;                       \
        os << "expected equality.\n"                 \
           << "__a: " << #A << " is " << __a << ")"  \
           << "\n != \n"                             \
           << "__b: " << #B << " is " << __b << ")"; \
        panic (os.str ());                           \
    });                                              \
} while (0)


#define check_neq(A,B) do {                          \
    const auto __a = (A);                            \
    const auto __b = (B);                            \
    _check_meta (!almost_equal (__a, __b),           \
                 { ; },                              \
                 {                                   \
        std::ostringstream os;                       \
        os << "unexpected equality.\n"               \
           << "__a: " << #A << " is " << __a << ")"  \
           << "\n == \n"                             \
           << "__b: " << #B << " is " << __b << ")"; \
        panic (os.str ());                           \
    });                                              \
} while (0)


#define check_throws(E,C) do {              \
    bool caught = false;                    \
                                            \
    try                                     \
        { C; }                              \
    catch (E)                               \
        { caught = true; }                  \
                                            \
    if (unlikely (!caught))                 \
        panic ("expected exception: " #E);  \
} while (0)

#define check(C) check_hard(C)


class panic_error {
    protected:
        std::string m_what;

    public:
        panic_error (const std::string &_what):
                m_what (_what)
        { ; }
};


void panic (const std::string&) terminal;
void panic (void)               terminal;


void not_implemented (void) terminal;
void unreachable     (void) terminal;
void unreachable     (const std::string&) terminal;
void unusual         (void);


void breakpoint (void);

#endif // __DEBUG_HPP
