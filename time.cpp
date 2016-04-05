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

#include "./time.hpp"

#include "./debug.hpp"
#include "./log.hpp"
#include "./platform.hpp"
#include "./cast.hpp"

#include <chrono>

using util::delta_clock;

///////////////////////////////////////////////////////////////////////////////
static const uint64_t SECOND      = 1'000'000'000UL;
static const uint64_t MILLISECOND =     1'000'000UL;


///////////////////////////////////////////////////////////////////////////////
uintmax_t
util::nanoseconds (void)
{
    return std::chrono::duration_cast<
        std::chrono::duration<uintmax_t, std::nano>
    > (
        std::chrono::high_resolution_clock::now ().time_since_epoch ()
    ).count ();
}


///////////////////////////////////////////////////////////////////////////////
delta_clock::delta_clock ():
    time { util::nanoseconds (), util::nanoseconds () }
{ ; }


//-----------------------------------------------------------------------------
float
delta_clock::seconds (void)
{
    time.prev = time.curr;
    time.curr = nanoseconds ();

    return (time.curr - time.prev) / static_cast<float> (SECOND);
}


///////////////////////////////////////////////////////////////////////////////
util::period_query::period_query (float seconds)
{
    m_time.start  = nanoseconds ();
    m_time.period = static_cast<uint64_t> (seconds * SECOND);
}


//-----------------------------------------------------------------------------
bool
util::period_query::poll (void)
{
    uint64_t now  = nanoseconds ();
    uint64_t diff = now - m_time.start;
    if (diff < m_time.period)
        return false;

    m_time.start += diff % m_time.period;
    return true;
}


///////////////////////////////////////////////////////////////////////////////
util::rate_limiter::rate_limiter (unsigned rate):
    m_last (nanoseconds ()),
    m_target (SECOND / rate)
{ ; }


//-----------------------------------------------------------------------------
void
util::rate_limiter::poll (void)
{
    uint64_t now = nanoseconds ();
    uint64_t total = now - m_last;

    if (total < m_target)
        sleep (m_target - total);

    m_last = now;
}


///////////////////////////////////////////////////////////////////////////////
util::polled_duration::polled_duration (std::string name, uint64_t interval):
    m_name     (name),
    m_interval (interval),
    m_next     (nanoseconds () + interval)
{ ; }


//-----------------------------------------------------------------------------
void
util::polled_duration::start (void)
{
    m_last = nanoseconds ();
}


//-----------------------------------------------------------------------------
void
util::polled_duration::stop (void) {
    uint64_t now = nanoseconds ();
    uint64_t dt  = now - m_last;
    m_series.add (dt / MILLISECOND);

    if (m_next < now) {
        LOG_DEBUG ("timing: '%s'. %s", m_name, m_series);
        m_series.reset ();
        m_next = now + m_interval;
    }
}
