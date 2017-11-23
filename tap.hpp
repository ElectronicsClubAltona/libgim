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

#ifndef __UTIL_TAP_HPP
#define __UTIL_TAP_HPP

#include <functional>

namespace util { namespace TAP {
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
        ~logger ();

        //---------------------------------------------------------------------
        template <typename ...Args, size_t N>
        void expect (bool, const char (&fmt)[N], Args&&...);

        template <typename ...Args, size_t N>
        void expect (const std::function<bool(Args...)>&, Args&&..., const char (&msg)[N]);

        //---------------------------------------------------------------------
        template <typename T, typename U, typename ...Args, size_t N>
        void expect_eq (const T&, const U&, const char (&fmt)[N], Args&&...);

        template <typename T, typename U, typename ...Args, size_t N>
        void expect_neq (const T&, const U&, const char (&fmt)[N], Args&&...);

        //---------------------------------------------------------------------
        template <typename T, typename U, typename ...Args, size_t N>
        void expect_gt (const T&, const U&, const char (&fmt)[N], Args&&...);

        template <typename T, typename U, typename ...Args, size_t N>
        void expect_ge (const T&, const U&, const char (&fmt)[N], Args&&...);

        template <typename T, typename U, typename ...Args, size_t N>
        void expect_lt (const T&, const U&, const char (&fmt)[N], Args&&...);

        template <typename T, typename U, typename ...Args, size_t N>
        void expect_le (const T&, const U&, const char (&fmt)[N], Args&&...);

        //---------------------------------------------------------------------
        template <typename T, typename ...Args, size_t N>
        void expect_nan (const T&, const char (&fmt)[N], Args&&...);

        //---------------------------------------------------------------------
        template <typename T, typename ...Args, size_t N>
        void expect_nothrow (T&&, const char (&fmt)[N], Args&&...);

        template <typename E, typename T, typename ...Args, size_t N>
        void expect_throw (T&&, const char (&fmt)[N], Args&&...);

        //---------------------------------------------------------------------
        template <size_t N, typename ...Args>
        void fail (const char (&fmt)[N], Args &&...args)
        {
            expect (false, fmt, std::forward<Args> (args)...);
        }

        void skip (const std::string &msg);
        void todo (const std::string &msg);
        void noop (void);

        //---------------------------------------------------------------------
        int status (void) const;

    private:
        int m_status;
        size_t m_size;
    };
} }

#include "tap.ipp"

#endif
