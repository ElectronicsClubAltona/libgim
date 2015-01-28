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

#include "maths.hpp"

#include <stdexcept>
#include <iostream>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////
#ifdef ENABLE_DEBUGGING
#define DEBUG_ONLY(X) do {  \
    X                       \
} while (0)
#else
#define DEBUG_ONLY(X)
#endif


///////////////////////////////////////////////////////////////////////////////
#define EXIT_XSUCCESS        0
#define EXIT_XSKIP          77
#define EXIT_XHARD_ERROR    99


///////////////////////////////////////////////////////////////////////////////
#define TRACE {                                                                     \
    DEBUG_ONLY(                                                                     \
        std::cerr << __FILE__ << ":" << __func__ << ":" << __LINE__ << std::endl;   \
    );                                                                              \
}


#define WARN(C) do {                                                                                \
    DEBUG_ONLY(                                                                                     \
        if (C) {                                                                                    \
            std::cerr << __FILE__ << ":" << __func__ << ":" << __LINE__ << ", " << #C << std::endl; \
        }                                                                                           \
    );                                                                                              \
} while (0)


///////////////////////////////////////////////////////////////////////////////
#define _CHECK_META(C, SUCCESS, FAILURE) do {                       \
    const auto __DEBUG_value = (C);                                 \
    if (!__DEBUG_value)            {                                \
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


///////////////////////////////////////////////////////////////////////////////
#define CHECK(C) do { DEBUG_ONLY(_CHECK_META((C), { ; }, { panic (); });); } while (0)

///////////////////////////////////////////////////////////////////////////////
#define CHECK_EQ(A,B) do {                                  \
    DEBUG_ONLY(                                             \
        const auto __a = (A);                               \
        const auto __b = (B);                               \
        _CHECK_META (almost_equal (__a, __b),               \
                     { ; },                                 \
                     {                                      \
            std::ostringstream __debug_os;                  \
            __debug_os.precision (15);                      \
            __debug_os                                      \
               << "expected equality.\n"                    \
               << "__a: " << #A << " is " << __a << ")"     \
               << "\n != \n"                                \
               << "__b: " << #B << " is " << __b << ")";    \
            panic (__debug_os.str ());                      \
        });                                                 \
    );                                                      \
} while (0)


///////////////////////////////////////////////////////////////////////////////
#define CHECK_LT(A,B) do {                                  \
    DEBUG_ONLY(                                             \
        const auto __a = (A);                               \
        const auto __b = (B);                               \
        _CHECK_META (__a < __b,                             \
                     { ; },                                 \
                     {                                      \
            std::ostringstream __debug_check_lt_os;         \
            __debug_check_lt_os                             \
               << "expected less than.\n"                   \
               << "__a: " << #A << " is " << __a << ")"     \
               << "\n >= \n"                                \
               << "__b: " << #B << " is " << __b << ")";    \
            panic (__debug_check_lt_os.str ());             \
        });                                                 \
    );                                                      \
} while (0)


///////////////////////////////////////////////////////////////////////////////
#define CHECK_LE(A,B) do {                                  \
    DEBUG_ONLY(                                             \
        const auto __a = (A);                               \
        const auto __b = (B);                               \
        _CHECK_META (__a <= __b,                            \
                     { ; },                                 \
                     {                                      \
            std::ostringstream __debug_check_lt_os;         \
            __debug_check_lt_os                             \
               << "expected less or equal to\n"             \
               << "__a: " << #A << " is " << __a << ")"     \
               << "\n > \n"                                 \
               << "__b: " << #B << " is " << __b << ")";    \
            panic (__debug_check_lt_os.str ());             \
        });                                                 \
    );                                                      \
} while (0)


///////////////////////////////////////////////////////////////////////////////
#define CHECK_GT(A,B) do {                                  \
    DEBUG_ONLY(                                             \
        const auto __a = (A);                               \
        const auto __b = (B);                               \
        _CHECK_META (__a > __b,                             \
                     { ; },                                 \
                     {                                      \
            std::ostringstream __debug_check_gt_os;         \
            __debug_check_gt_os                             \
               << "expected greater than.\n"                \
               << "__a: " << #A << " is " << __a << ")"     \
               << "\n <= \n"                                \
               << "__b: " << #B << " is " << __b << ")";    \
            panic (__debug_check_gt_os.str ());             \
        });                                                 \
    );                                                      \
} while (0)


///////////////////////////////////////////////////////////////////////////////
#define CHECK_GE(A,B) do {                                  \
    DEBUG_ONLY(                                             \
        const auto __a = (A);                               \
        const auto __b = (B);                               \
        _CHECK_META (__a >= __b,                            \
                     { ; },                                 \
                     {                                      \
            std::ostringstream __debug_check_gt_os;         \
            __debug_check_gt_os                             \
               << "expected greater or equal to.\n"         \
               << "__a: " << #A << " is " << __a << ")"     \
               << "\n < \n"                                 \
               << "__b: " << #B << " is " << __b << ")";    \
            panic (__debug_check_gt_os.str ());             \
        });                                                 \
    );                                                      \
} while (0)


///////////////////////////////////////////////////////////////////////////////
#define CHECK_NEQ(A,B) do {                                 \
    DEBUG_ONLY(                                             \
        const auto __a = (A);                               \
        const auto __b = (B);                               \
        _CHECK_META (!almost_equal (__a, __b),              \
                     { ; },                                 \
                     {                                      \
            std::ostringstream __debug_neq_os;              \
            __debug_neq_os << "unexpected equality.\n"      \
               << "__a: " << #A << " is " << __a << ")"     \
               << "\n == \n"                                \
               << "__b: " << #B << " is " << __b << ")";    \
            panic (__debug_neq_os.str ());                  \
        });                                                 \
    );                                                      \
} while (0)


///////////////////////////////////////////////////////////////////////////////
#define CHECK_THROWS(E,C) do {                  \
    DEBUG_ONLY(                                 \
        bool caught = false;                    \
                                                \
        try                                     \
            { C; }                              \
        catch (E)                               \
            { caught = true; }                  \
                                                \
        if (!caught)                            \
            panic ("expected exception: " #E);  \
    );                                          \
} while (0)


///////////////////////////////////////////////////////////////////////////////
#define CHECK_NOTHROW(C) do {               \
    DEBUG_ONLY(                             \
        try {                               \
            C;                              \
        } catch (...) {                     \
            panic ("unexpected exception"); \
        }                                   \
    );                                      \
} while (0)


///////////////////////////////////////////////////////////////////////////////
class panic_error {
    protected:
        std::string m_what;

    public:
        panic_error (const std::string &_what):
                m_what (_what)
        { ; }
};


void panic [[noreturn]] (const std::string&);
void panic [[noreturn]] (void);


///////////////////////////////////////////////////////////////////////////////
void not_implemented [[noreturn]] (void);
void not_implemented [[noreturn]] (const char*);


///////////////////////////////////////////////////////////////////////////////
void unreachable [[noreturn]] (void);
void unreachable [[noreturn]] (const std::string&);
void unusual (void);


///////////////////////////////////////////////////////////////////////////////
void breakpoint (void);


///////////////////////////////////////////////////////////////////////////////
void enable_fpe (void);
void disable_fpe (void);


///////////////////////////////////////////////////////////////////////////////
namespace debug {
    void init (void);
}

#endif // __DEBUG_HPP
