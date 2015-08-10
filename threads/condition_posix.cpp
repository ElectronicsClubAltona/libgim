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

#include "condition.hpp"

#include "except.hpp"

using util::threads::condition;
using util::threads::mutex;


///////////////////////////////////////////////////////////////////////////////
condition::condition ()
{
    auto err = pthread_cond_init (&m_id, nullptr);
    errno_error::try_code (err);
}


//-----------------------------------------------------------------------------
condition::~condition ()
{
    auto err = pthread_cond_destroy (&m_id);
    errno_error::try_code (err);
}


///////////////////////////////////////////////////////////////////////////////
void
condition::signal (void)
{
    auto err = pthread_cond_signal (&m_id);
    errno_error::try_code (err);
}


//-----------------------------------------------------------------------------
void
condition::broadcast (void)
{
    auto err = pthread_cond_broadcast (&m_id);
    errno_error::try_code (err);
}


///////////////////////////////////////////////////////////////////////////////
void
condition::wait (mutex &m)
{
    auto mutid = m.native ();
    auto err = pthread_cond_wait (&m_id, &mutid);
    errno_error::try_code (err);
}


//-----------------------------------------------------------------------------
void
condition::wait (mutex &m, std::chrono::nanoseconds t)
{
    struct timespec abstime;
    abstime.tv_sec  = std::chrono::duration_cast<std::chrono::seconds> (t).count ();
    abstime.tv_nsec = std::chrono::duration_cast<std::chrono::nanoseconds> (t).count ();

    auto mutid = m.native ();

    auto err = pthread_cond_timedwait (&m_id, &mutid, &abstime);
    errno_error::try_code (err);
}
