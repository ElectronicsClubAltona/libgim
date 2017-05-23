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
 * Copyright 2010-2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_TIME_HPP
#define __UTIL_TIME_HPP

#include <chrono>
#include <cstdint>
#include <string>

#include "./stats.hpp"

namespace util {
    // ------------------------------------------------------------------------
    uintmax_t nanoseconds (void);

    template <typename T>
    void sleep (std::chrono::duration<T,std::nano>);

    void sleep (uint64_t ns);

    // ------------------------------------------------------------------------
    class delta_clock {
        public:
            delta_clock ();
            float seconds (void);

        protected:
            struct {
                uint64_t prev;
                uint64_t curr;
            } time;
    };

    // ------------------------------------------------------------------------
    class period_query {
        public:
            explicit period_query (float seconds);

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
            explicit rate_limiter (unsigned rate);

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

#include "./time.ipp"

#endif // __UTIL_TIME_HPP
