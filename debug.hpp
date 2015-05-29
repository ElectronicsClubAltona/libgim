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
 * Copyright 2010-2012 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __DEBUG_HPP
#define __DEBUG_HPP

//#include "maths.hpp" // XXX: See notes at the end of file for maths.hpp inclusion
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
#define CHECK_LIMIT(VAL,LO,HI) do {                                 \
    DEBUG_ONLY(                                                     \
        const auto __val = (VAL);                                   \
        const auto __hi  = (HI);                                    \
        const auto __lo  = (LO);                                    \
                                                                    \
        _CHECK_META (__val >= __lo && __val <= __hi,                \
                     { ; },                                         \
                     {                                              \
            std::ostringstream __os;                                \
            __os << "expected satisifies limit\n"                   \
                 << "__val: " << #VAL << " = " << __val << '\n'     \
                 << "__lo: " << #LO << " = " << __lo << '\n'        \
                 << "__hi: " << #HI << " = " << __hi << '\n';       \
            panic (__os.str ());                                    \
        });                                                         \
    );                                                              \
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
#define CHECK_ZERO(A) do {                                  \
    DEBUG_ONLY(                                             \
        const auto __a = (A);                               \
        _CHECK_META (almost_zero (__a),                     \
                     { ; },                                 \
                     {                                      \
            std::ostringstream __debug_nez_os;              \
            __debug_nez_os << "expected zero.\n"            \
               << "__a: " << #A << " is " << __a << ")";    \
            panic (__debug_nez_os.str ());                  \
        });                                                 \
    );                                                      \
} while (0)


///////////////////////////////////////////////////////////////////////////////
#define CHECK_NEZ(A) do {                                   \
    DEBUG_ONLY(                                             \
        const auto __a = (A);                               \
        _CHECK_META (!almost_zero (__a),                    \
                     { ; },                                 \
                     {                                      \
            std::ostringstream __debug_nez_os;              \
            __debug_nez_os << "unexpected zero.\n"          \
               << "__a: " << #A << " is " << __a << ")";    \
            panic (__debug_nez_os.str ());                  \
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


    template <typename T>
    bool valid (const T&);


    template <
        template<size_t, typename...> class T,
        size_t S,
        typename ...Args
    >
    struct validator {
        static bool is_valid (const T<S,Args...>&);
    };


    template <
        template<size_t,typename...> class T,
        size_t S,
        typename ...Args
    >
    bool valid (const T<S,Args...> &v)
    { return validator<T,S,Args...>::is_valid (v); }


    template <typename T>
    void sanity (const T &t)
    {
        (void)t;
        CHECK (valid (t));
    }


    template <
        template<typename...> class T,
        typename ...Args
    >
    void sanity (const T<Args...> &t)
    {
        (void)t;
        CHECK (valid (t));
    }
}


///////////////////////////////////////////////////////////////////////////////
// XXX: maths needs to be included so that CHECK_EQ/NEQ can call almost_equal,
// but maths.hpp might be using CHECK_ macros so we must include maths.hpp
// after we define the CHECK_ macros so the preprocessor can resolve them.
#include "maths.hpp"

#endif // __DEBUG_HPP
