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
#include <vector>
#include <initializer_list>

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
        template <typename ...Args>
        void expect (bool, const std::string &fmt, Args&&...);

        template <typename ...Args>
        void expect (const std::function<bool(Args...)>&, Args&&..., const std::string& msg);

        //---------------------------------------------------------------------
        template <typename T, typename U, typename ...Args>
        void expect_eq (const T&, const U&, const std::string &fmt, Args&...);

        template <typename T, typename U, typename ...Args>
        void expect_neq (const T&, const U&, const std::string &fmt, Args&...);

        //---------------------------------------------------------------------
        template <typename T, typename U, typename ...Args>
        void expect_gt (const T&, const U&, const std::string &fmt, Args&...);

        template <typename T, typename U, typename ...Args>
        void expect_ge (const T&, const U&, const std::string &fmt, Args&...);

        template <typename T, typename U, typename ...Args>
        void expect_lt (const T&, const U&, const std::string &fmt, Args&...);

        template <typename T, typename U, typename ...Args>
        void expect_le (const T&, const U&, const std::string &fmt, Args&...);

        //---------------------------------------------------------------------
        template <typename T, typename ...Args>
        void expect_nan (const T&, const std::string &fmt, Args&...);

        //---------------------------------------------------------------------
        template <typename T, typename ...Args>
        void expect_nothrow (T&&, const std::string &fmt, Args&...);

        template <typename E, typename T, typename ...Args>
        void expect_throw (T&&, const std::string &fmt, Args&...);

        //---------------------------------------------------------------------
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
