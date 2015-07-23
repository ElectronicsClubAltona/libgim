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
 * Copyright 2010 Danny Robson <danny@nerdcruft.net>
 */

#include "time.hpp"

#include "platform.hpp"

#if !defined(PLATFORM_WIN32)
#error
#endif

#include <windows.h>

static const uint64_t SECOND      = 1'000'000'000UL;
static const uint64_t MILLISECOND =     1'000'000UL;

///////////////////////////////////////////////////////////////////////////////
uint64_t
util::nanoseconds (void)
{
    LARGE_INTEGER freq, count;
    QueryPerformanceFrequency (&freq);
    QueryPerformanceCounter   (&count);

    return count.QuadPart * SECOND / freq.QuadPart;
}


///////////////////////////////////////////////////////////////////////////////
void
util::sleep (uint64_t ns)
{
    Sleep (ns / MILLISECOND);
}
