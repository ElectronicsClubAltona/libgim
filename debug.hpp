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

#ifdef ENABLE_DEBUGGING
#define DEBUG_ONLY(X) \
    X;
#else
#define DEBUG_ONLY(X)
#endif

#define TRACE {                                                                \
    std::cerr << __FILE__ << ":" << __func__ << ":" << __LINE__ << std::endl;  \
}


#define WARN(MSG) do {                                                                      \
   std::cerr << __FILE__ << ":" << __func__ << ":" __LINE__ << ", " << (MSG) << std::endl;  \
} while (0)


#define WARN_IF(C, MSG) do {                                                                    \
    if (C) {                                                                                    \
        std::cerr << __FILE__ << ":" << __func__ << ":" __LINE__ << ", " << (MSG) << std::endl; \
    }                                                                                           \
} while (0)


#define VERIFY_SOFT(C, COND) ({     \
    const auto __DEBUG_value = (C); \
    CHECK_SOFT(__DEBUG_value COND); \
    __DEBUG_value;                  \
})


#define VERIFY_HARD(C, COND) ({     \
    const auto __DEBUG_value = (C); \
    CHECK_HARD(__DEBUG_value COND); \
    __DEBUG_value;                  \
})


#define _CHECK_META(C, SUCCESS, FAILURE) do {                       \
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


#if defined(ENABLE_DEBUGGING)
    #define CHECK_HARD(C) _CHECK_META((C), { ; }, { panic ();      })
    #define CHECK_SOFT(C) _CHECK_META((C), { ; }, { breakpoint (); })
    #define CHECK(C) CHECK_HARD(C)
#else
    #define CHECK_HARD(C) { };
    #define CHECK_SOFT(C) { };
    #define CHECK(C) CHECK_SOFT(C)
#endif


#define CHECK_EQ(A,B) do {                           \
    const auto __a = (A);                            \
    const auto __b = (B);                            \
    _CHECK_META (almost_equal (__a, __b),            \
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


#define CHECK_NEQ(A,B) do {                          \
    const auto __a = (A);                            \
    const auto __b = (B);                            \
    _CHECK_META (!almost_equal (__a, __b),           \
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


#define CHECK_THROWS(E,C) do {              \
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
void not_implemented (const char*) terminal;

void unreachable     (void) terminal;
void unreachable     (const std::string&) terminal;
void unusual         (void);


void breakpoint (void);


void enable_fpe (void);
void disable_fpe (void);

#endif // __DEBUG_HPP
