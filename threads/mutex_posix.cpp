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

#include "mutex.hpp"

#include "../except.hpp"
#include "../debug.hpp"

using util::threads::mutex;


///////////////////////////////////////////////////////////////////////////////
mutex::mutex ()
{
    auto err = pthread_mutex_init (&m_id, nullptr);
    errno_error::try_code (err);
}


//-----------------------------------------------------------------------------
mutex::~mutex ()
{
    auto err = pthread_mutex_destroy (&m_id);
    errno_error::try_code (err);
}


///////////////////////////////////////////////////////////////////////////////
void
mutex::lock (void)
{
    auto err = pthread_mutex_lock (&m_id);
    errno_error::try_code (err);
}


//-----------------------------------------------------------------------------
bool
mutex::try_lock (void)
{
    switch (auto err = pthread_mutex_trylock (&m_id)) {
    case 0:     return true;
    case EBUSY: return false;

    default:
        errno_error::throw_code (err);
    }

    unreachable ();
}
