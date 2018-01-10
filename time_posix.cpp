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

#include "time.hpp"

#include "cast.hpp"

#include <ctime>

static constexpr uint64_t SECOND = 1'000'000'000UL;

///////////////////////////////////////////////////////////////////////////////
void
util::sleep (uint64_t ns)
{
    struct timespec req, rem;

    req.tv_sec  = trunc_cast<time_t> (ns / SECOND);
    req.tv_nsec = trunc_cast<long  > (ns % SECOND);

    while (nanosleep (&req, &rem)) {
        req = rem;
    }
}
