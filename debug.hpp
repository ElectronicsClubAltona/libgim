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
 * Copyright 2010-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __DEBUG_HPP
#define __DEBUG_HPP

//#include "maths.hpp" // XXX: See notes at the end of file for maths.hpp inclusion
#include <stdexcept>
#include <string>


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
#define _CHECK_PANIC(FMT,...) do {                          \
    panic ("%s:%s:%i:%s\n" FMT,                             \
           PACKAGE, __FILE__, __LINE__, __FUNCTION__,       \
           __VA_ARGS__);                                    \
} while(0)


///////////////////////////////////////////////////////////////////////////////
#define CHECK(C) do {   \
    DEBUG_ONLY(         \
        if (!(C))       \
            panic ();   \
    );                  \
} while (0)

///////////////////////////////////////////////////////////////////////////////
#define CHECK_EQ(A,B) do {                                  \
    DEBUG_ONLY(                                             \
        const auto __a = (A);                               \
        const auto __b = (B);                               \
                                                            \
        if (!util::almost_equal (__a, __b)) {               \
            _CHECK_PANIC("expected equality\n"              \
                         "__a: %s is %!\n"                  \
                         "__b: %s is %!\n",                 \
                         #A, __a,                           \
                         #B, __b);                          \
        }                                                   \
    );                                                      \
} while (0)


///////////////////////////////////////////////////////////////////////////////
#define CHECK_LT(A,B) do {                                  \
    DEBUG_ONLY(                                             \
        const auto __a = (A);                               \
        const auto __b = (B);                               \
                                                            \
        if (__a >= __b) {                                   \
            _CHECK_PANIC("expected less than\n"             \
                          "__a: %s is %!\n"                 \
                          "__b: %s is %!\n",                \
                          #A, __a,                          \
                          #B, __b);                         \
        };                                                  \
    );                                                      \
} while (0)


///////////////////////////////////////////////////////////////////////////////
#define CHECK_LE(A,B) do {                                  \
    DEBUG_ONLY(                                             \
        const auto __a = (A);                               \
        const auto __b = (B);                               \
                                                            \
        if (__a > __b) {                                    \
            _CHECK_PANIC("expected less than or equal\n"    \
                         "__a: %s is %!\n"                  \
                         "__b: %s is %!\n",                 \
                         #A, __a,                           \
                         #B, __b);                          \
        }                                                   \
    );                                                      \
} while (0)


///////////////////////////////////////////////////////////////////////////////
#define CHECK_GT(A,B) do {                                  \
    DEBUG_ONLY(                                             \
        const auto __a = (A);                               \
        const auto __b = (B);                               \
                                                            \
        if (__a <= __b) {                                   \
            _CHECK_PANIC ("expected greater than\n"         \
                          "__a: %s is %!\n"                 \
                          "__b: %s is %!\n",                \
                          #A, __a,                          \
                          #B, __b);                         \
        }                                                   \
    );                                                      \
} while (0)


///////////////////////////////////////////////////////////////////////////////
#define CHECK_GE(A,B) do {                                  \
    DEBUG_ONLY(                                             \
        const auto __a = (A);                               \
        const auto __b = (B);                               \
                                                            \
        if (__a < __b) {                                    \
            _CHECK_PANIC ("expected greater or equal\n"     \
                          "__a: %s is %!\n"                 \
                          "__b: %s is %!\n",                \
                          #A, __a,                          \
                          #B, __b);                         \
        };                                                  \
    );                                                      \
} while (0)


///////////////////////////////////////////////////////////////////////////////
#define CHECK_LIMIT(V,L,H) do {                             \
    DEBUG_ONLY(                                             \
        const auto __v = (V);                               \
        const auto __l = (L);                               \
        const auto __h = (H);                               \
                                                            \
        if (__v < __l || __v > __h) {                       \
            _CHECK_PANIC ("expected limit\n"                \
                          "__l: %s is %!\n"                 \
                          "__h: %s is %!\n"                 \
                          "__v: %s is %!\n",                \
                          #H, __h,                          \
                          #L, __l,                          \
                          #V, __v);                         \
        };                                                  \
    );                                                      \
} while (0)

///////////////////////////////////////////////////////////////////////////////
#define CHECK_NEQ(A,B) do {                                 \
    DEBUG_ONLY(                                             \
        const auto __a = (A);                               \
        const auto __b = (B);                               \
                                                            \
        if (util::almost_equal (__a, __b)) {                \
            _CHECK_PANIC ("expected inequality\n"           \
                          "__a: %s is %s\n"                 \
                          "__b: %s is %s\n",                \
                          #A, __a,                          \
                          #B, __b);                         \
        };                                                  \
    );                                                      \
} while (0)


///////////////////////////////////////////////////////////////////////////////
#define CHECK_ZERO(A) do {                                  \
    DEBUG_ONLY(                                             \
        const auto __a = (A);                               \
                                                            \
        if (!util::almost_zero (__a)) {                     \
            _CHECK_PANIC ("expected zero\n"                 \
                          "__a: %s is %s\n"                 \
                          #A, __a);                         \
        };                                                  \
    );                                                      \
} while (0)


///////////////////////////////////////////////////////////////////////////////
#define CHECK_NEZ(A) do {                                   \
    DEBUG_ONLY(                                             \
        const auto __a = (A);                               \
        if (util::exactly_zero (__a))                       \
            _CHECK_PANIC ("expected zero\n"                 \
                          "__a: %s is %!",                  \
                          #A, __a);                         \
    );                                                      \
} while (0)


///////////////////////////////////////////////////////////////////////////////
#define CHECK_THROWS(E,C) do {                              \
    DEBUG_ONLY(                                             \
        bool caught = false;                                \
                                                            \
        try                                                 \
            { C; }                                          \
        catch (E)                                           \
            { caught = true; }                              \
                                                            \
        if (!caught)                                        \
            _CHECK_PANIC ("expected exception: %s", #E);    \
    );                                                      \
} while (0)


///////////////////////////////////////////////////////////////////////////////
#define CHECK_NOTHROW(C) do {                               \
    DEBUG_ONLY(                                             \
        try {                                               \
            C;                                              \
        } catch (const std::exception &e) {                 \
            _CHECK_PANIC ("unexpected exception: %s",       \
                          e.what ());                       \
        } catch (...) {                                     \
            _CHECK_PANIC ("unexpected exception: %s",       \
                          "unknown");                       \
        }                                                   \
    );                                                      \
} while (0)


///////////////////////////////////////////////////////////////////////////////
constexpr void panic [[noreturn]] (const char*);
constexpr void panic [[noreturn]] (void);

template <typename ...Args>
constexpr void panic [[noreturn]] (const char *fmt, const Args&...);


///////////////////////////////////////////////////////////////////////////////
constexpr void not_implemented [[noreturn]] (void);
constexpr void not_implemented [[noreturn]] (const char *msg);


///////////////////////////////////////////////////////////////////////////////
constexpr void unreachable [[noreturn]] (void);
constexpr void unreachable [[noreturn]] (const char*);


///////////////////////////////////////////////////////////////////////////////
void warn (void);
void warn (const std::string&);
void warn (const char *);


///////////////////////////////////////////////////////////////////////////////
void breakpoint (void);


///////////////////////////////////////////////////////////////////////////////
void enable_fpe (void);
void disable_fpe (void);


///////////////////////////////////////////////////////////////////////////////
namespace util { namespace debug {
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
} }


#include "./debug.ipp"

///////////////////////////////////////////////////////////////////////////////
// XXX: maths needs to be included so that CHECK_EQ/NEQ can call almost_equal,
// but maths.hpp might be using CHECK_ macros so we must include maths.hpp
// after we define the CHECK_ macros so the preprocessor can resolve them.
#include "./maths.hpp"


#endif // __DEBUG_HPP
