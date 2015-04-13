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

        logger ();
        ~logger ();

        void expect (bool, const std::string &msg);

        template <typename T, typename U>
        void expect (std::function<bool(const T&, const U&)> test, const T&, const U&, const std::string &msg);

        template <typename T, typename U>
        void expect_eq (const T&, const U&, const std::string &msg = "equality");
        template <typename T, typename U>
        void expect_neq (const T&, const U&, const std::string &msg = "inequality");

        template <typename T, typename U> void expect_gt (const T&, const U&, const std::string &msg = "gt");
        template <typename T, typename U> void expect_ge (const T&, const U&, const std::string &msg = "ge");
        template <typename T, typename U> void expect_lt (const T&, const U&, const std::string &msg = "lt");
        template <typename T, typename U> void expect_le (const T&, const U&, const std::string &msg = "le");

        void skip (const std::string &msg);
        void todo (const std::string &msg);
        void noop (void);

        int status (void) const;

    private:
        int m_status;
        size_t m_size;
    };
} }

#include "tap.ipp"

#endif
