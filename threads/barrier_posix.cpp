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

#include "barrier.hpp"

#include "except.hpp"

using util::threads::barrier;


///////////////////////////////////////////////////////////////////////////////
barrier::barrier (unsigned count)
{
    pthread_barrier_init (&m_id, nullptr, count);
}


//-----------------------------------------------------------------------------
barrier::~barrier ()
{
    pthread_barrier_destroy (&m_id);
}


///////////////////////////////////////////////////////////////////////////////
void
barrier::wait (void)
{
    auto err = pthread_barrier_wait (&m_id);
    errno_error::try_code (err);
}
