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

#include "mutex_win32.hpp"

#include "except.hpp"

using util::threads::win32::mutex;


///////////////////////////////////////////////////////////////////////////////
mutex::mutex ():
    m_id (CreateMutex (nullptr, false, nullptr))
{ ; }


///////////////////////////////////////////////////////////////////////////////
bool
mutex::lock (void)
{
    switch (WaitForSingleObject (m_id, INFINITE)) {
    case WAIT_OBJECT_0:
        return true;

    case WAIT_FAILED:
        win32_error::throw_code ();
    default:
        unreachable ();
    }
}


//-----------------------------------------------------------------------------
bool
mutex::lock (std::chrono::nanoseconds ns)
{
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds> (ns).count ();
    switch (WaitForSingleObject (m_id, ms)) {
    case WAIT_OBJECT_0:
        return true;
    case WAIT_TIMEOUT:
        return false;

    case WAIT_FAILED:
        win32_error::throw_code ();
    default:
        unreachable ();
    }
}


///////////////////////////////////////////////////////////////////////////////
void
mutex::unlock (void)
{
    if (ReleaseMutex (m_id))
        win32_error::throw_code ();
}
