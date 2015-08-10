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

#include "spinlock.hpp"

#include "except.hpp"

using util::threads::spinlock;


///////////////////////////////////////////////////////////////////////////////
spinlock::spinlock ()
{
    auto err = pthread_spin_init (&m_id, PTHREAD_PROCESS_PRIVATE);
    errno_error::try_code (err);
}


//-----------------------------------------------------------------------------
spinlock::~spinlock ()
{
    auto err = pthread_spin_destroy (&m_id);
    errno_error::try_code (err);
}


///////////////////////////////////////////////////////////////////////////////
void
spinlock::lock (void)
{
    auto err = pthread_spin_lock (&m_id);
    errno_error::try_code (err);
}


//-----------------------------------------------------------------------------
void
spinlock::unlock (void)
{
    auto err = pthread_spin_unlock (&m_id);
    errno_error::try_code (err);
}
