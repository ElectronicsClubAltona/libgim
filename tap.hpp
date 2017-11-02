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
 * Copyright 2015-2017 Danny Robson <danny@nerdcruft.net>
 */

#ifndef CRUFT_UTIL_TAP_HPP
#define CRUFT_UTIL_TAP_HPP

#include "format.hpp"
#include "maths.hpp"

#include <cmath>
#include <ostream>
#include <functional>

namespace util::TAP {
    /// A simple TAP (Test Anything Protocol) test case output
    class logger {
    public:
        enum {
            PASS,
            FAIL,
            SKIP,
            TODO
        };

        //---------------------------------------------------------------------
        logger ();
        logger (std::ostream&);
        ~logger ();

        //---------------------------------------------------------------------
        template <typename ...Args, size_t N>
        void
        expect (const bool test, const char (&fmt)[N], Args&&... args)
        {
            m_output << (test ? "ok " : "not ok ") << ++m_size
                     << " - "
                     << util::format::render (fmt, std::forward<Args> (args)...) << '\n';

            if (!test)
                m_status = EXIT_FAILURE;
        }


        //---------------------------------------------------------------------
        template <typename ...Args, size_t N>
        void
        expect (const std::function<bool(Args...)> &test, Args&&...args, const char (&fmt)[N])
        {
            try {
                expect (test (std::forward<Args> (args)...), fmt);
            } catch (...) {
                expect (false, fmt);
            }
        }


        ///////////////////////////////////////////////////////////////////////
        template <typename T, typename U, typename ...Args, size_t N>
        void
        expect_eq (const T &a, const U &b, const char (&fmt)[N], Args&&...args)
        {
            expect (almost_equal (a, b), fmt, std::forward<Args> (args)...);
        }


        //---------------------------------------------------------------------
        template <typename T, typename U, typename ...Args, size_t N>
        void
        expect_neq (const T &a, const U &b, const char (&fmt)[N], Args&&...args)
        {
            expect (!almost_equal (a, b), fmt, std::forward<Args> (args)...);
        }


        ///////////////////////////////////////////////////////////////////////
        template <typename ValueA, typename ValueB, typename ...Args, size_t N>
        void
        expect_gt (const ValueA &a, const ValueB &b, const char (&fmt)[N], Args&&...args)
        {
            expect (a > b, fmt, std::forward<Args> (args)...);
        }


        //---------------------------------------------------------------------
        template <typename T, typename U, typename ...Args, size_t N>
        void
        expect_ge (const T &a, const U &b, const char (&fmt)[N], Args&&...args)
        {
            expect (a >= b, fmt, std::forward<Args> (args)...);
        }


        //---------------------------------------------------------------------
        template <typename T, typename U, typename ...Args, size_t N>
        void
        expect_lt (const T &a, const U &b, const char (&fmt)[N], Args&&...args)
        {
            expect (a < b, fmt, std::forward<Args> (args)...);
        }


        //---------------------------------------------------------------------
        template <typename T, typename U, typename ...Args, size_t N>
        void
        expect_le (const T &a, const U &b, const char (&fmt)[N], Args&&...args)
        {
            expect (a <= b, fmt, std::forward<Args> (args)...);
        }


        ///////////////////////////////////////////////////////////////////////
        template <typename T, typename ...Args, size_t N>
        void
        expect_nan (const T &t, const char (&fmt)[N], Args&&...args)
        {
            expect (std::isnan (t), fmt, std::forward<Args> (args)...);
        }


        ///////////////////////////////////////////////////////////////////////
        template <typename T, typename ...Args, size_t N>
        void
        expect_nothrow (T &&t, const char (&fmt)[N], Args&&...args)
        {
            bool success = true;

            try {
                t ();
            } catch (...) {
                success = false;
            }

            expect (success, fmt, std::forward<Args> (args)...);
        }


        //---------------------------------------------------------------------
        template <typename E, typename T, typename ...Args, size_t N>
        void
        expect_throw (T &&t, const char (&fmt)[N], Args&&...args)
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


        ///////////////////////////////////////////////////////////////////////
        template <size_t N, typename ...Args>
        void fail (const char (&fmt)[N], Args &&...args)
        {
            expect (false, fmt, std::forward<Args> (args)...);
        }


        //---------------------------------------------------------------------
        void skip (const std::string &msg);
        void todo (const std::string &msg);
        void noop (void);


        ///////////////////////////////////////////////////////////////////////
        int status (void) const;


    private:
        std::ostream &m_output;
        int m_status;
        size_t m_size;
    };
}

#endif
