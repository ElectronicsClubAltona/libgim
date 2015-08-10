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

#include "semaphore.hpp"

#include "debug.hpp"
#include "except.hpp"

using util::threads::semaphore;


///////////////////////////////////////////////////////////////////////////////
semaphore::semaphore (unsigned value)
{
    auto err = sem_init (&m_id, false, value);
    errno_error::try_code (err);
}


//-----------------------------------------------------------------------------
semaphore::~semaphore ()
{
    auto err = sem_destroy (&m_id);
    errno_error::try_code (err);
}


///////////////////////////////////////////////////////////////////////////////
void
semaphore::wait (void)
{
    auto err = sem_wait (&m_id);
    errno_error::try_code (err);
}


//-----------------------------------------------------------------------------
bool
semaphore::try_wait (void)
{
    switch (auto err = sem_wait (&m_id)) {
    case 0:      return true;
    case EAGAIN: return false;

    default:
        errno_error::throw_code (err);
    }

    unreachable ();
}


//-----------------------------------------------------------------------------
void
semaphore::post (void)
{
    auto err = sem_post (&m_id);
    errno_error::try_code (err);
}
