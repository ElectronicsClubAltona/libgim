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
 * Copyright 2010 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_TIME_HPP
#define __UTIL_TIME_HPP

#include <cstdint>
#include <string>

#include "stats.hpp"

namespace util {
    // ------------------------------------------------------------------------
    uint64_t nanoseconds (void);
    void     sleep (uint64_t ns);

    // ------------------------------------------------------------------------
    class delta_clock {
        public:
            delta_clock ();
            double seconds (void);

        protected:
            struct {
                uint64_t prev;
                uint64_t curr;
            } time;
    };

    // ------------------------------------------------------------------------
    class period_query {
        public:
            period_query (double seconds);

            bool poll (void);

        protected:
            struct {
                uint64_t start;
                uint64_t period;
            } m_time;
    };

    // ------------------------------------------------------------------------
    class rate_limiter {
        public:
            rate_limiter (unsigned rate);

            void poll (void);

        protected:
            uint64_t m_last;
            unsigned m_target;
    };

    // ------------------------------------------------------------------------
    class polled_duration {
        public:
            polled_duration (std::string name, uint64_t interval);

            void start (void);
            void stop  (void);

        protected:
            std::string m_name;
            uint64_t    m_last;
            uint64_t    m_interval;
            uint64_t    m_next;

            stats::accumulator<uint64_t> m_series;
    };
}

#endif // __UTIL_TIME_HPP
