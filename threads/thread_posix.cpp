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

#include "thread.hpp"

#include "except.hpp"

#include <unistd.h>

using util::threads::thread;


///////////////////////////////////////////////////////////////////////////////
void
thread::join (void)
{
    auto err = pthread_join (m_id, nullptr);
    errno_error::try_code (err);
}


//-----------------------------------------------------------------------------
void
thread::kill (void)
{
    auto err = pthread_cancel (m_id);
    errno_error::try_code (err);
}


///////////////////////////////////////////////////////////////////////////////
void
thread::yield (void)
{
    auto err = pthread_yield ();
    errno_error::try_code (err);
}


///////////////////////////////////////////////////////////////////////////////
unsigned
thread::concurrency (void)
{
    auto val = sysconf (_SC_NPROCESSORS_ONLN);
    if (val < 0)
        errno_error::throw_code ();

    return val;
}



///////////////////////////////////////////////////////////////////////////////
// pthread entry point
void*
thread::dispatch (void *func)
{
    typedef decltype(thread::m_func) func_t;
    (*reinterpret_cast<func_t*> (func)) ();
    return nullptr;
}
